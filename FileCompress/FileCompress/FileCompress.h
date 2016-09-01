#define _CRT_SECURE_NO_WARNINGS 0
#pragma once

#include<iostream>
#include<string>
#include<assert.h>
#include"Huffman.h"

using namespace std;

typedef unsigned long longType;

//ÿ���ڵ����Ϣ
struct CharInfo
{
	CharInfo(longType count = 0)
		:_count(count)
	{}
	bool operator!=(const CharInfo& info)const
	{
		return _count != info._count;
	}
	CharInfo operator+(const CharInfo& info)const
	{
		return CharInfo(_count + info._count);
	}
	bool operator<(const CharInfo& info)const
	{
		return _count < info._count;
	}
	unsigned char _ch;//ÿ������ŵ��ַ�
	longType _count;//���ַ����ֵĴ���
	string _code;//���ַ��Ĺ���������
};

class FileCompress
{
public:
	FileCompress()
	{
		for(size_t i = 0;i < 256;++i)
		{
			_infos[i]._ch = i;
			_infos[i]._count = 0;
		}
	}

	void ComPress(const char* filename)
	{
		FILE* fOut = fopen(filename,"rb");
		assert(fOut);

		//1.ͳ���ļ���ÿ���ַ����ֵĴ���
		char ch  = fgetc(fOut);

		while(!feof(fOut))
		{
			_infos[(unsigned char)ch]._count++;//�����������±�
			ch = fgetc(fOut);
		}
	
		//2.����Huffman��
		CharInfo invaild;
		HuffmanTree<CharInfo> tree (_infos,256,invaild);

		//3.����Huffman����
		string code;
		GenerateHuffmanCode(tree.GetRoot(),code);

		//4.ѹ��	
		string compressFileName = filename;
		compressFileName += ".compress";
		FILE* fIn = fopen(compressFileName.c_str(),"wb");
		assert(fIn);

		fseek(fOut,0,SEEK_SET);//
		ch = fgetc(fOut);
		char value = 0;
		int size = 0;

		while (!feof(fOut))
		{
			string& code = _infos[(unsigned char)ch]._code;
			for(size_t i = 0;i < code.size();++i)
			{
				if(code[i] == '1')
				{
					value |= 1;
				}
				++size;
				if(size == 8)
				{
					fputc(value,fIn);
					size = 0;
					value = 0;
				}
				value<<= 1;
			}
			ch = fgetc(fOut);
		}

		//��λ
		if (size > 0)
		{
			value <<= (7 - size);//��Ϊ���������һλ
			putc(value, fIn);
		}

		//5.д�����ļ�����¼�ַ�������ֵĴ���
		string configFileName = filename;
		configFileName += ".config";
		FILE* fconfig = fopen(configFileName.c_str(),"wb");
		assert(fconfig);

		string line;
		char buffer[128];
 		for(size_t i = 0;i < 256;++i)
		{
			if(_infos[i]._count >0)
			{
				line += _infos[i]._ch;
				line +=',';
				itoa(_infos[i]._count,buffer,10);
				line +=buffer;
				line +='\n';
			}

			//ѹ��ͼƬʱ��\0�ĸ����ж����fputsд����ȥ���赥���������������
			//��ͬʱ����Ϊ0�ģ����ַ������ó�\0,����Ӱ����
			if(line[0] == '\0')
			{
				fputc('\0', fconfig);
				fputc(',', fconfig);
				fputs(itoa(_infos[i]._count,buffer,10), fconfig);
				fputc('\n', fconfig);
				line.clear();
				continue;
			}

			fputs(line.c_str(),fconfig);
			line.clear();
		}

		fclose(fconfig);
		fclose(fOut);
		fclose(fIn);
	}

	bool ReadLine(FILE* fOut,string& line)
	{
		char ch = fgetc(fOut);
		//if (ch == EOF)
		if(feof(fOut))
		{
			return false;
		}
		

		//while(ch != EOF && ch != '\n')
		while(!feof(fOut) && ch != '\n')
		{
			line+=ch;
			ch = fgetc(fOut);
		}
		return true;
	}

	void UnCompress(const char* filename)
	{
		//1.��ȡ�����ļ����õ��ַ�������ֵĴ���
		string configFileName = filename;
		configFileName += ".config";
		FILE* fconfig = fopen(configFileName.c_str(),"rb");
		assert(fconfig);

		string line;
		longType charCount = 0;
		while(ReadLine(fconfig,line))
		{
			//ͳ�ƻ��з����ֵĴ���
			if(line.empty())
			{
				//����ȥ����һ�У������ĸ���������д��continue
				line +='\n';
			}
			else
			{
				unsigned char ch = line[0];
				_infos[ch]._count = atoi(line.substr(2).c_str());
				line.clear();
			}
		}

		//2.����huffmanTree
		CharInfo invalid;
		HuffmanTree<CharInfo> tree(_infos,256,invalid);
		charCount = tree.GetRoot()->_weight._count;

		//3.��ȡѹ���ļ������л�ԭ
		string compressFile = filename;
		compressFile += ".compress";
		FILE* fOut = fopen(compressFile.c_str(), "rb");
		assert(fOut);

		string uncompressFile = filename;
		uncompressFile += ".uncompress";
		FILE* fIn = fopen(uncompressFile.c_str(), "wb");
		assert(fIn);

		HuffmanTreeNode<CharInfo>* root = tree.GetRoot();
		HuffmanTreeNode<CharInfo>* cur = root;

		char ch = fgetc(fOut);
		int pos = 7;
	
		while(1)
		{
			if(ch & 1 << pos)
				cur = cur->_right;
			else
				cur = cur->_left;

			if(pos-- == 0)
			{
				ch = fgetc(fOut);
				pos =7;
			}
			if(cur->_left == NULL && cur->_right == NULL)
			{
				fputc(cur->_weight._ch,fIn);
				if (--charCount == 0)
				{
					break;
				}
				cur = root;
			}
		}

		fclose(fconfig);
		fclose(fOut);
		fclose(fIn);
	}
protected:
	void GenerateHuffmanCode(HuffmanTreeNode<CharInfo>* root,string code)
	{
		if(root == NULL)
		{
			return;
		}
		GenerateHuffmanCode(root->_left,code + '0');
		GenerateHuffmanCode(root->_right,code + '1');
		if(root->_left == NULL && root->_right == NULL)
		{
			_infos[root->_weight._ch]._code = code;
			return;
		}
	}
protected:
	CharInfo _infos[256];
};

	void TestFileCompress()
	{
		FileCompress fc;
		fc.ComPress("Input");
		//fc.ComPress("Input.BIG");
		//fc.ComPress("1.jpg");

	}

	void TestFileUncompress()
	{
		FileCompress fc;
		fc.UnCompress("Input");
		//fc.UnCompress("Input.BIG");
		//fc.UnCompress("1.jpg");
	}