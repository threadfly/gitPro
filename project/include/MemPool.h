#include "common.h"
#include "MemoryPool.h"


template <typename T, USHORT nInitSize=1024, USHORT nGrowSize=256>
class MemPool
{
	public:
		static void * operator new(size_t)
		{
			return mempool.Alloc();
		}

		static void operator delete(void *p)
		{
			mempool.Free(p);
		}

	private:
		static MemoryPool mempool;
};

template <typename T, USHORT nInitSize, USHORT nGrowSize>
MemoryPool MemPool<T, nInitSize, nGrowSize>::mempool(sizeof(T), nInitSize, nGrowSize);

