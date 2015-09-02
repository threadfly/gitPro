#ifndef _THREAD_COND_
#define _THREAD_COND_

#include <pthread.h>
#include <sys/time.h>
#include <time.h>

#include "thread_mutex.h"
#include "log/sync_log.h"


namespace Common
{

namespace Thread
{
	using namespace Common::Log;
	class Cond
	{
	public:
		Cond();
		~Cond();

		int Wait(Mutex &);

		int Signal();

		int WaitTime(Mutex &, int sec=0, int usec=0);

		int Broadcast();
	private:
		pthread_cond_t	m_cond;
	};

}

}

#endif
