#include "common.h"
#include "a.h"
#include "MemoryPool.h"
#include <sys/time.h>

#define TEST_TIMES	800000
A* A_array[TEST_TIMES];
int main()
{
	timeval start,end;
    cout<<"sizeof(USHORT):"<<sizeof(USHORT)<<endl;

	cout<<"-------- Use Memory Pool --------"<<endl;
	//MemoryPool pool(sizeof(A));
	MemoryPool pool(sizeof(A), 800000, 5000);
	gettimeofday(&start, NULL);
	for (USHORT i = 0; i < TEST_TIMES; i++)
	{
		//MemoryPool pool(2);
		A * pA = new (pool.Alloc()) A;
		A_array[i] = pA;
		//pA->~A();
	//	pool.Free(pA);
	}
	gettimeofday(&end, NULL);

	for (USHORT i = 0; i < TEST_TIMES; i++)
	{
		//MemoryPool pool(2);
		A_array[i]->~A();
		pool.Free(A_array[i]);
	//	pool.Free(pA);
	}

	USHORT timeuser = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
	cout<<"-- Use Memory Pool -- cost:"<< timeuser/1000 <<" microsecond"<<endl;

	cout<<"------- No Use Memory Pool -------"<<endl;
	gettimeofday(&start, NULL);
	for (USHORT i = 0; i < TEST_TIMES; i++)
	{
		A * pA = new A;
		A_array[i] = pA;
	}
	gettimeofday(&end, NULL);
	timeuser = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
	for (USHORT i = 0; i < TEST_TIMES; i++)
	{
		delete A_array[i];
	}
	cout<<"-- No Use Memory Pool -- cost:"<< timeuser/1000 <<" microsecond"<<endl;
	return 0;
}

