#ifndef _ID_GENERATOR_
#define _ID_GENERATOR_

#include "thread/thread_mutex.h"

namespace Common
{

namespace Tools
{
	using namespace Common::Thread;
	class IDGenerator
	{
	public:
		IDGenerator();

		long GetNewID();
	private:
		long	m_id;
		Mutex	m_mutex;
	};

}

}

#endif
