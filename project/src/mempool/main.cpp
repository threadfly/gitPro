#include "common.h"
#include "MemoryPool.h"

class A
{
public:
	A(){}
	~A(){}
private:
	USHORT a;
};

int main()
{
	MemoryPool pool(sizeof(A));
	A * pA = new (pool.Alloc()) A;
	return 0;
}

