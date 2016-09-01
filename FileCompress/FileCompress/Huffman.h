#pragma once
#include<iostream>
#include"Heap.h"

using namespace std;

template<class T>
struct HuffmanTreeNode
{
	HuffmanTreeNode*  _left;
	HuffmanTreeNode* _right;
	T _weight;

	HuffmanTreeNode(const T& x)
		:_weight(x)
		,_left(NULL)
		,_right(NULL)
	{
	}
};

template<class T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
public:
	HuffmanTree(const T* a,size_t n,const T& invaild)
	{
		struct NodeCompare
		{
			bool operator() (const Node* l,const Node* r)
			{
				return l->_weight < r->_weight;
			}
		};

		Heap<Node*,NodeCompare> minHeap;
		for(size_t i = 0; i < n;++i)
		{
			if(a[i] != invaild)
			{
				minHeap.Push(new Node(a[i]));
			}
		}

		while(minHeap.Size() > 1)
		{
			//取出概率最小的两个节点
			Node* left = minHeap.Top();
			minHeap.Pop();
			Node* right = minHeap.Top();
			minHeap.Pop();

			//构造新节点
			Node* parent = new Node(left->_weight + right->_weight);
			parent->_left = left;
			parent->_right = right;

			minHeap.Push(parent);
		}
		_root = minHeap.Top();
	}

	~HuffmanTree()
	{
		_Destory(_root);
		_root = NULL;
	}
   Node* GetRoot()
   {
	   return _root;
   }
protected:
	HuffmanTree(const HuffmanTree& ht);
	HuffmanTree& operator= (const HuffmanTree& ht);
protected:
void _Destory(Node* root)
{
	if(root == NULL)
	{
		return ;
	}
	_Destory(root->_left);
	_Destory(root->_right);
	delete root;
}
protected:
	Node* _root;
};

void TestHuffman()
{
	//int a[] = {2,4,0,5,6,7};
	int a[] = {0,1,2,3,4,5,6,7,8,9};
	HuffmanTree<int> ht(a,sizeof(a) /sizeof(a[0]),0);
}