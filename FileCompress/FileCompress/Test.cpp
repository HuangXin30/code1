#include"FileCompress.h"
#include<Windows.h>

using namespace std;

void Test()
{	
	int begin = GetTickCount();
	TestFileCompress();
	int end = GetTickCount();

	cout<<"ѹ�����õ�ʱ�䣺"<<end - begin <<"ms"<<endl;

	begin = GetTickCount();
	TestFileUncompress();
	end = GetTickCount();

	cout<<"��ѹ�����õ�ʱ�䣺"<<end - begin<<"ms"<<endl;
}
int main()
{
	 //TestHuffman();
	//TestHeap();
	//TestFileCompress();
	//TestFileUncompress();
	Test();
	getchar();
	return 0;
}