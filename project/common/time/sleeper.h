#ifndef _SLEEPER_
#define _SLEEPER_

#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <stdio.h>

namespace Common
{

namespace Time
{
	class Sleeper
	{
	public:
		Sleeper(long sec, long usec);

		void Start();
	private:
		long m_sec;
		long m_usec;
		struct timeval	m_time;
	};

}
}

#endif
