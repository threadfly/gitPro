#ifndef _MEMORY_BLOCK_
#define _MEMORY_BLOCK_

#include "common.h"

struct MemoryBlock {
	USHORT			nSize;
	USHORT			nFree;
	USHORT			nFirst;
	USHORT			nDummyAlign1;
	MemoryBlock *	pNext;
	char			aData[1];

	static void * operator new(size_t, USHORT nTypes, USHORT nUnitSize)
	{
		return ::operator new(sizeof(MemoryBlock) + nTypes * nUnitSize);
	}

	static void operator delete(void *p, size_t)
	{
		::operator delete (p);
	}

	MemoryBlock (USHORT nTypes = 1, USHORT nUnitSize = 0);
	~MemoryBlock(){}
};

#endif

