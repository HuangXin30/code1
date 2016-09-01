#pragma once

#include<iostream>
#include<vector>
#include<assert.h>

using namespace std;

//仿函数
template<class T>
struct Less
{
	bool operator()(const T& left,const T& right)
	{
		return  left < right;
	}
};

template<class T>
struct Greater
{
	bool operator()(const T& left,const T& right)
	{
		return  left > right;
	}
};
  
template <class T,class Compare=Greater<T>>
class Heap
{
public:
	Heap()
	{}

	Heap(const T* a,size_t size)
	{
		assert(a);

		for(size_t i = 0;i < size;i++)
		{
			_a.push_back(a[i]);
		}

		//建堆
		for(int i = (_a.size() - 2) / 2;i >= 0;--i)
		{
			//从最后一个非叶子结点开始
			_AdjustDown(i);
		}
	}

	~Heap()
	{}

	void Push(const T& x)
	{
		_a.push_back(x);
		_AdjustUp((_a.size() - 1));
	}

	//删除最大的数
	void Pop()
	{
		assert(!_a.empty());

		swap(_a[0],_a[_a.size() - 1]);
		_a.pop_back();

		_AdjustDown(0);
	}

	size_t Size()
	{
		return _a.size();
	}

	T Top()
	{
		return _a[0];
	}

	bool Empty()
	{
		return _a.empty();
	}
protected:
	void _AdjustDown(size_t parent)
	{
		//Compare com;
		//向下调整
		size_t child= parent * 2 +1;
		while(child <_a.size())
		{
			//找较大的孩子节点
			if(child + 1 < _a.size() 
				&&Compare()( _a[child+ 1] , _a[child] ))
			{
				++child;
			}

			//如果大于父节点，交换并继续
			if(Compare()( _a[child] , _a[parent] ))
			{
				swap(_a[child],_a[parent]);
				parent = child;
				child = parent * 2 +1;
			}
			else
			{
				break;
			}
		}
	}

	void _AdjustUp(int child)
	{
		int parent =(child - 1) /2 ;
	    while(parent >= 0)
		{
			if(Compare()( _a[child] , _a[parent] ))
			{
				swap(_a[parent],_a[child]);
				child = parent;
				parent = (child - 1) /2 ;
			}
			else
			{
				break;
			}
		}
	}
protected:
	vector<T> _a;
};




template<class T>
class PriorityQueue
{
public:
	PriorityQueue()
	{}

	void Pop()
	{
		_hp.Pop();
	}

	void Push(const T& x)
	{
		_hp.Push(x);
	}

protected:
	Heap<T> _hp;
};


void AdjustDown(int *a,int n,int parent)
{
	int child = parent * 2 +1;
	while(child < n)
	{
		if(child+1 <n&&a[child+1] >a[child] )
		{
			++child;
		}
		if(a[child] > a[parent])
		{
			swap(a[child] , a[parent]);
			parent = child;
			child = parent * 2+1;
		}
		else
		{
			break;
		}
	}
}
void HeapSort(int *a,int n)
{
	assert(a);
	for(int i= (n-2) / 2;i>=0;--i)
	{
		AdjustDown(a,n,i);
	} 
	for(size_t i = 0;i < n;++i)
	{
		swap(a[0],a[n-1-i]);
		AdjustDown(a,n-1-i,0);
	}
}



void TestHeap()
{
	int a[] = {10,11,13,12,16,18,15,17,14,19};
	Heap<int> hp1(a,sizeof(a)/sizeof(a[0]));
	hp1.Push(20);
	hp1.Pop();
}

void TestHeapSort()
{
	int a[] = {10,11,13,12,16,18,15,17,14,19};
	HeapSort(a,sizeof(a)/sizeof(a[0]));
}

