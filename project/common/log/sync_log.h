#ifndef _SYNC_LOG_
#define _SYNC_LOG_

#include <stdarg.h>

#include "common_log.h"

namespace Common
{

namespace Log
{
	class SyncLog
	{
	public:
		static void LOG(LEVEL,const char * info, ...);
	};
}

}

#endif

