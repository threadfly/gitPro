#ifndef _MEMORY_BLOCK_
#define _MEMORY_BLOCK_

#include "common.h"
#include <iostream>


struct MemoryBlock {
	USHORT			nSize;
	USHORT			nFree;
	USHORT			nFirst;
	USHORT			nDummyAlign1;
	MemoryBlock *	pNext;
	char			aData[1];

	static void * operator new(size_t size, USHORT nTypes, USHORT nUnitSize)
	{
        #ifdef DEBUG
        //    cout<<"Call MemoryBlock::operator new( ...... )"<< endl;
        #endif
        //cout<<"sizeof(MemoryBlock):"<<sizeof(MemoryBlock)<<endl;
		return ::operator new(sizeof(MemoryBlock) + nTypes * nUnitSize);
	}

	//static void operator delete(void *p, size_t)
	static void operator delete(void *p)
	{
        #ifdef DEBUG
        //   cout<<"Call MemoryBlock::operator delete(void *p)"<< endl;
        #endif
		::operator delete (p);
	}

	MemoryBlock (USHORT nTypeNum = 1, USHORT nUnitSize = 0);
	~MemoryBlock(){}
};

#endif

