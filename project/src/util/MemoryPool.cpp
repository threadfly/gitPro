#include "MemoryPool.h"

MemoryPool::MemoryPool(USHORT _nUnitSize, USHORT _nInitSize, USHORT _nGrowSize)
{
	// 1.
	pBlock 		= NULL;
	// 2.
	nInitSize 	= _nInitSize;
	// 3.
	nGrowSize 	= _nGrowSize;

    if (_nUnitSize < sizeof(USHORT))
    {
        cout<<"_nUnitSize < sizeof(USHORT)):"<<"_nUnitSize:"<<_nUnitSize<<" "<<"sizeof(USHORT):"<<sizeof(USHORT)<<endl;
        abort();
        //exit(EXIT_FAILURE);
    }

	if (_nUnitSize > 4)
	{
		// 4. 将大于4字节的大小_nUnitSize往上取整大于_nUnitSize的最小的MEMPOOL_ALIGNMENT的倍数(前提是MEMPOOL_ALIGNMENT为2的整数倍)
		nUnitSize = (_nUnitSize + (MEMPOOL_ALIGNMENT-1))& ~(MEMPOOL_ALIGNMENT-1);
	}
	else if (_nUnitSize <= 2)
	{
		// 5.最少2个字节来保存编号
		nUnitSize = 2;
	}
	else
	{
		nUnitSize = 4;
	}
}

void * MemoryPool::Alloc()
{
    bool first = false;
	if (!pBlock)	// 1
	{// pBlock is NULL, So we need new an newly MemoryBlock
		/* code */
		pBlock = new(nInitSize, nUnitSize)MemoryBlock(nInitSize,nUnitSize);
        first = true;
	}

	// Find an MemoryBlock that has empty space!
	MemoryBlock * pMyBlock = pBlock;
	while(pMyBlock && !pMyBlock->nFree) // 2
		pMyBlock = pMyBlock->pNext;

	if (pMyBlock) // 3
	{	// Return the address from finded MemoryBlock
        if (first)
        {
            char * pFree = pMyBlock->aData;
            pMyBlock->nFirst = 1;

            return (void*)pFree;
        }
        else
        {
            char * pFree = pMyBlock->aData+(pMyBlock->nFirst*nUnitSize);
            pMyBlock->nFirst = *((USHORT*)pFree);

            pMyBlock->nFree--;
            return (void*)pFree;
            
        }
	}
	else	// 4
	{	// All MemoryBlock is Full!!
		if (!nGrowSize)
		{
			return NULL;
		}
		// So we need new an newly MemoryBlock
		pMyBlock = new(nGrowSize, nUnitSize)MemoryBlock(nGrowSize, nUnitSize);

		if (!pMyBlock)
			return NULL;

		// We should put newly MemoryBlock into first position at MemoryBlock link, which
		// can fasten search the MemoryBlock that has empty space
		pMyBlock->pNext = pBlock;
		pBlock = pMyBlock;

		return (void *)(pMyBlock->aData);
	}
}

void MemoryPool::Free(void * pFree)
{
	MemoryBlock * pMyBlock = pBlock;
    if (pBlock == NULL)
        return;
	// Find the address pFree belong to which MemoryBlock
	// tips: here code is verify that malloc heap is from
	// low address to high address
	MemoryBlock * pPreBlock = pBlock;
	while ( ((ULONG)pMyBlock->aData > (ULONG)pFree )||
			((ULONG)pFree >= ((ULONG)pMyBlock->aData + pMyBlock->nSize)))
	{// 1
		pPreBlock = pMyBlock;
		pMyBlock = pMyBlock->pNext;
        if (pMyBlock == NULL)
            break;
	}

	if (pFree == NULL)
		return ;

	pMyBlock->nFree++;
	*((USHORT*)pFree) = pMyBlock->nFirst;
	pMyBlock->nFirst = (USHORT)(((ULONG)pFree-(ULONG)(pMyBlock->aData))/ nUnitSize);

	if (pMyBlock->nFree*nUnitSize == pMyBlock->nSize)
	{
        if (pPreBlock == pMyBlock)
            pBlock = NULL;
		pPreBlock->pNext = pMyBlock->pNext;
		//delete pMyBlock;
        pMyBlock->~MemoryBlock();
        //MemoryBlock::operator delete(pMyBlock, sizeof(MemoryBlock)+ nInitSize*nUnitSize );
        MemoryBlock::operator delete(pMyBlock);
	}
	
}

MemoryPool::~MemoryPool()
{
	MemoryBlock * pMyBlock = pBlock;
	while( pMyBlock)
	{
		//delete pMyBlock;
        MemoryBlock * pTmp = pMyBlock->pNext;
        
        // 这条操作应该相当于他下面的操作才对
        delete pMyBlock;
        /*
        pMyBlock->~MemoryBlock();
        //MemoryBlock::operator delete(pMyBlock, sizeof(MemoryBlock)+ nInitSize*nUnitSize );
        MemoryBlock::operator delete(pMyBlock);
        */
        pMyBlock = pTmp;
	}
}
