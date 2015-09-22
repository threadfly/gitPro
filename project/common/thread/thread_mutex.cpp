#include "thread_mutex.h"

namespace Common 
{

namespace Thread
{

	Mutex::Mutex()
	{
		//m_mutex = PTHEAD_MUTEX_INITIALIZER;
		/*int ret = pthread_mutexattr_settype(&m_attr, MUTEX_ATTR);
		if ( 0 != ret )
		{
			SyncLog::LOG(EROR, "pthread_mutexattr_settype error | ret:%d", ret);
		}*/
		int ret = pthread_mutexattr_init(&m_attr);
		if ( 0 != ret )
		{
			SyncLog::LOG(EROR, "pthread_mutexattr_init error | ret:%d", ret);
			perror("Thread MutexAttr");
		}

		ret = ::pthread_mutex_init(&m_mutex, &m_attr);
		if ( 0 != ret )
		{
			SyncLog::LOG(EROR, "pthread_mutex_init error");
			perror("Thread Mutex");
		}
	}
	
	Mutex::~Mutex()
	{
		int ret = ::pthread_mutex_destroy(&m_mutex);
		if ( 0 != ret )
		{
			//perror("~Mutex mutex destroy error");
			SyncLog::LOG(EROR, "~Mutex mutex destroy error | ret:%d", ret);
		}

		ret = ::pthread_mutexattr_destroy(&m_attr);
		if ( 0 != ret )
		{
			SyncLog::LOG(EROR, "pthread_mutexattr_destroy error");
		}
	}

	int Mutex::Lock()
	{
		int ret = ::pthread_mutex_lock(&m_mutex);
		if ( 0 != ret )
		{
			//perror("pthread_mutex_lock error");
			SyncLog::LOG(EROR, "pthread_mutex_lock error | ret:%d", ret);
		}
		return ret;
	}

	int Mutex::UnLock()
	{
		int ret = ::pthread_mutex_unlock(&m_mutex);
		if ( 0 != ret )
		{
			SyncLog::LOG(EROR, "pthread_mutex_unlock error | ret:%d", ret);
			//perror("pthread_mutex_unlock error");
		}
		return ret;
	}

	int Mutex::TryLock()
	{
		int ret = ::pthread_mutex_trylock(&m_mutex);
		if ( 0 != ret)
		{
			SyncLog::LOG(EROR, "pthread_mutex_trylock error | ret:%d", ret);
		}
		return ret;
	}
} // namespace Thread

} // namespace Common


