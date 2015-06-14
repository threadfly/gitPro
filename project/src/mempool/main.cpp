#include "common.h"
#include "a.h"
#include "MemoryPool.h"

int main()
{
    cout<<"sizeof(USHORT):"<<sizeof(USHORT)<<endl;
	//MemoryPool pool(sizeof(A));
	MemoryPool pool(2);
	A * pA = new (pool.Alloc()) A;
    pA->~A();
    pool.Free(pA);
	return 0;
}

