#ifndef _THREAD_MUTEX_
#define _THREAD_MUTEX_

#include <pthread.h>

#include "log/sync_log.h"

namespace Common 
{

namespace Thread
{
	using namespace Common::Log;
	/* MUTEX_ATTR:
	 *	PTHREAD_MUTEX_TIMED_NP (default)
	 *	PTHREAD_MUTEX_RECURSIVE_NP
	 *	PTHREAD_MUTEX_ERRORCHECK_NP
	 *	PTHREAD_MUTEX_ADAPTIVE_NP
	 */
	#define MUTEX_ATTR PTHREAD_MUTEX_TIMED_NP
	class Mutex
	{
	public:
		Mutex();
		~Mutex();
		
		int Lock();

		int UnLock();
		
		int TryLock();
	public:
		pthread_mutexattr_t	m_attr;
		pthread_mutex_t	    	m_mutex;
	};

}

}

#endif

