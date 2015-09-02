#include "thread_mutex.h"

namespace Common 
{

namespace Thread
{

	Mutex::Mutex()
	{
		//m_mutex = PTHEAD_MUTEX_INITIALIZER;
		int ret = pthread_mutexattr_settype(&m_attr, MUTEX_ATTR);
		if ( 0 != ret)
		{
			SyncLog::LOG(EROR, "pthread_mutexattr_settype error");
		}

		ret = ::pthread_mutex_init(&m_mutex, &m_attr);
		if (0 != ret)
		{
			SyncLog::LOG(EROR, "pthread_mutex_init error");
		}
	}
	
	Mutex::~Mutex()
	{
		int ret = ::pthread_mutex_destroy(&m_mutex);
		if ( ret != 0)
		{
			//perror("~Mutex mutex destroy error");
			SyncLog::LOG(EROR, "~Mutex mutex destroy error");
		}
	}

	int Mutex::Lock()
	{
		int ret = ::pthread_mutex_lock(&m_mutex);
		if (ret != 1)
		{
			//perror("pthread_mutex_lock error");
			SyncLog::LOG(EROR, "pthread_mutex_lock error");
		}
		return ret;
	}

	int Mutex::UnLock()
	{
		int ret = ::pthread_mutex_unlock(&m_mutex);
		if (ret != 1)
		{
			SyncLog::LOG(EROR, "pthread_mutex_unlock error");
			//perror("pthread_mutex_unlock error");
		}
		return ret;
	}

	int Mutex::TryLock()
	{
		int ret = ::pthread_mutex_trylock(&m_mutex);
		if ( 0 != ret)
		{
			SyncLog::LOG(EROR, "pthread_mutex_trylock error");
		}
		return ret;
	}
} // namespace Thread

} // namespace Common


