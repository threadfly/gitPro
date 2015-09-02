#include "sleeper.h"

namespace Common
{

namespace Time
{

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

