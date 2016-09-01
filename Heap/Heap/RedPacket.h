#pragma once

#include<iostream>
#include<assert.h>
#include<time.h>

using namespace std;

//红包问题，从1000000个中找出100个最大的

const int N = 100000;
const int K = 100;


void AdjustDownRed(int topK[],int size,int parent)
{
	assert(topK);

	int child = parent * 2 + 1;

	while(child < size)
	{
		if(child + 1<size && topK[child+1] <topK[child])
		{
			++child;
		}

		if(topK[child] <topK[parent])
		{
			swap(topK[child],topK[parent]);
			parent = child;
			child = parent * 2+1;
		}
		else
		{
			break;
		}
	}
}
void GetTopK(int a[],int topK[])
{
	assert(K < N);
	for(int i = 0; i< K;++i)
	{
		topK[i] = a[i];
	}

	//建小堆
	for(int i = (K-2)/2;i>=0;--i)
	{
		AdjustDownRed(topK,K,i);
	}
	for (int i = K; i < N; ++i)
	{
		if (a[i] > topK[0])
		{
			topK[0] = a[i];
			AdjustDownRed(topK, K, 0);
		}
	}
	for (int i = 0; i < K; ++i)
	{
		cout<<topK[i]<<" ";
	}

	cout<<endl;
}
void TestTopK()
{
	int a[N], topK[K];
	srand(time(0));
	for (int i = 0; i < N; ++i)
	{
		a[i] = rand() % 10000;
	}

	for (int i = N - K; i < N; ++i)
	{
		a[i] = 10000+i;
	}

	GetTopK(a, topK);
}