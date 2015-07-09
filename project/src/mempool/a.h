/*************************************************************************
	> File Name: a.h
	> Author: WuFF
	> Mail: 294815470@qq.com
	> Created Time: Sun 14 Jun 2015 04:31:52 AM PDT
 ************************************************************************/

#ifndef _A_H
#define _A_H

#include "common.h"
#include "MemPool.h"

class A:public MemPool<A, 8000, 1024>
{
public:
    A();
    ~A();
private:
    USHORT a;
	USHORT b_array[1000];
};


#endif
