#ifndef _SLEEPER_
#define _SLEEPER_
#include "sys/select.h"
#include "sys/time.h"
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

	Sleeper::Sleeper(long sec, long usec):
	m_sec(sec),
	m_usec(usec)
	{
	}

	void Sleeper::Start()
	{	
		m_time.tv_sec = m_sec;
		m_time.tv_usec = m_usec;
		int err = 0;
		do {
			if ((err = select(1, NULL, NULL, NULL, &m_time) < 0))
			{
				perror("Sleeper start");
			}
		}while(err < 0 && errno == EINTR);
	}

}
}

#endif
