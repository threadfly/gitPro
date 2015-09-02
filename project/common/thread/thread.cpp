#include "thread.h"
#include "errno.h"

namespace Common {

namespace Thread {

Thread::Thread(bool isDetach):m_isDetach(isDetach)
{
	if (isDetach){
		pthread_attr_setdetachstate(&m_attr, PTHREAD_CREATE_DETACHED);
	}
	pthread_attr_init(&m_attr);
}

Thread::~Thread()
{
	pthread_attr_destroy(&m_attr);
}

int Thread::Start()
{
	int err;
	err = pthread_create(&m_pid, &m_attr, Thread::Loop, (void*)this);
	if ( err != 0 )
	{
		// TODO
		if ( err == EAGAIN)
		{
		} else if ( err == EINVAL)
		{
		} else if ( err == EPERM)
		{
		}
	}
	return err;
}

bool Thread::IsSelf()
{ 
	return (m_pid == pthread_self());
}

bool Thread::Join()
{
	if (false == m_isDetach)
	{
		if (pthread_join(m_pid, NULL) > 0)
			return false;
		return true;
	}
	return false;
}

} // namespace Thread

} // namespace Common

