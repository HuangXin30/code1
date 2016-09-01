#define _CRT_SECURE_NO_WARNINGS 0
#pragma once

#include<iostream>
#include<string>
#include<assert.h>
#include"Huffman.h"

using namespace std;

typedef unsigned long longType;

//每个节点的信息
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
	unsigned char _ch;//每个结点存放的字符
	longType _count;//此字符出现的次数
	string _code;//此字符的哈夫曼编码
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

		//1.统计文件中每个字符出现的次数
		char ch  = fgetc(fOut);

		while(!feof(fOut))
		{
			_infos[(unsigned char)ch]._count++;//负数不能做下标
			ch = fgetc(fOut);
		}
	
		//2.构造Huffman树
		CharInfo invaild;
		HuffmanTree<CharInfo> tree (_infos,256,invaild);

		//3.生成Huffman编码
		string code;
		GenerateHuffmanCode(tree.GetRoot(),code);

		//4.压缩	
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

		//补位
		if (size > 0)
		{
			value <<= (7 - size);//因为最后还左移了一位
			putc(value, fIn);
		}

		//5.写配置文件，记录字符及其出现的次数
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

			//压缩图片时，\0的个数有多个，fputs写不进去，需单独处理这种情况，
			//但同时个数为0的，其字符都被置成\0,但不影响结果
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
		//1.读取配置文件，得到字符及其出现的次数
		string configFileName = filename;
		configFileName += ".config";
		FILE* fconfig = fopen(configFileName.c_str(),"rb");
		assert(fconfig);

		string line;
		longType charCount = 0;
		while(ReadLine(fconfig,line))
		{
			//统计换行符出现的次数
			if(line.empty())
			{
				//继续去读下一行，是它的个数，不能写成continue
				line +='\n';
			}
			else
			{
				unsigned char ch = line[0];
				_infos[ch]._count = atoi(line.substr(2).c_str());
				line.clear();
			}
		}

		//2.构建huffmanTree
		CharInfo invalid;
		HuffmanTree<CharInfo> tree(_infos,256,invalid);
		charCount = tree.GetRoot()->_weight._count;

		//3.读取压缩文件，进行还原
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