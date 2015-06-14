#ifndef _MEMORY_POOL_
#define _MEMORY_POOL_

#include "MemoryBlock.h"
#include "common.h"
#include "stdlib.h"

class MemoryPool
{
public:
	MemoryPool(USHORT nUnitSize, USHORT nInitSize=1024, USHORT nGrowSize = 256);
	~MemoryPool();

	void *			Alloc();
	void			Free(void * p);

private:
	MemoryBlock * 	pBlock;
	USHORT			nUnitSize;
	USHORT			nInitSize;
	USHORT			nGrowSize;

};
#endif

