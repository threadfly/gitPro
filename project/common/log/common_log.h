#ifndef _COMMON_LOG_
#define _COMMON_LOG_

#include <stdio.h>
#include <string.h>
#include <iostream>

#include <sys/time.h>
using namespace std;

namespace Common
{

namespace Log
{
	enum LEVEL
	{
		DEBUG = 1,
		INFO,
		EROR
	};
}

}

#endif

