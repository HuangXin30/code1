#include"FileCompress.h"
#include<Windows.h>

using namespace std;

void Test()
{	
	int begin = GetTickCount();
	TestFileCompress();
	int end = GetTickCount();

	cout<<"压缩所用的时间："<<end - begin <<"ms"<<endl;

	begin = GetTickCount();
	TestFileUncompress();
	end = GetTickCount();

	cout<<"解压缩所用的时间："<<end - begin<<"ms"<<endl;
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