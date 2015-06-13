#include "MemoryBlock.h"

//进一步的结论：既可以在类的声明中，也可以在函数定义中声明缺省参数，但不能既在类声明中又在函数定义中同时声明缺省参数。
MemoryBlock::MemoryBlock(USHORT nTypes ,USHORT nUnitSize)
:nSize(nTypes * nUnitSize),
nFree (nTypes - 1),
nFirst (1),
pNext (0)
{
		char * pData = aData;
		for (USHORT i = 1; i < nTypes; i++)
		{
			*reinterpret_cast<USHORT*>(pData) = i;
			pData += nUnitSize;
		}
}


