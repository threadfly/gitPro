#include "thread_cond.h"

namespace Common
{

namespace Thread
{

	Cond::Cond()
	{
		//m_cond = PTHREAD_COND_INITIALIZER;
		int ret = ::pthread_cond_init(&m_cond, NULL);
		if ( 0 != ret)
		{
			SyncLog::LOG(EROR, "pthread_cond_init error | ret:%d", ret);
		}
	}
	
	Cond::~Cond()
	{
		int ret = ::pthread_cond_destroy(&m_cond);
		if ( 0 != ret)
		{
			SyncLog::LOG(EROR, "pthread_cond_destroy error | ret:%d", ret);
		}
	}

	int Cond::Wait(Mutex & mutex)
	{
		int ret = ::pthread_cond_wait(&m_cond, &mutex.m_mutex);
		if ( 0 != ret )
		{
			SyncLog::LOG(EROR, "pthread_cont_wait error | ret:%d", ret);
			//perror("pthread_cont_wait error");
		}
		return ret;
	}
	
	int Cond::Signal()
	{
		int ret = ::pthread_cond_signal(&m_cond);
		if ( 0 != ret)
		{
			SyncLog::LOG(EROR, "pthread_cont_signal error | ret:%d", ret);
			//perror("pthread_cont_signal error");
		}
		return ret;
	}

	int Cond::WaitTime(Mutex & mutex, int sec, int usec)
	{
		if (0 == sec && 0 == usec)
			return Wait(mutex);
		//Here Can Optimization
		struct timeval tv;
		struct timespec ts;
		if (gettimeofday(&tv, NULL) < 0)
		{
			SyncLog::LOG(EROR, "gettimeofday error");
			//perror("gettimeofday error");
		}
		ts.tv_sec = tv.tv_sec + sec;
		ts.tv_nsec = tv.tv_usec + usec;
		int ret = ::pthread_cond_timedwait(&m_cond, &mutex.m_mutex, &ts);
		if ( ret != 0 )
		{
			SyncLog::LOG(EROR, "pthread_cond_timewait error | ret:%d", ret);
			//perror("pthread_cond_timewait error");
		}
		return ret;
	}

	int Cond::Broadcast()
	{
		int ret = ::pthread_cond_broadcast(&m_cond);
		if ( 0 != ret)
		{
			SyncLog::LOG(EROR, "pthread_cond_broadcast error | ret%d", ret);
			//perror("pthread_cond_broadcast error");
		}
		return ret;
	}
}  // namespace Thread

} // namespace Common

