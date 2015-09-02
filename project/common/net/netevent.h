#ifndef _NET_EVENT_
#define _NET_EVENT_
#include "net.h"

#define TIMEOUT_SEC 0
#define TIMEOUT_USEC 400000
namespace Common
{

namespace Net
{
	//#define EVENT_BUF 255
	const int EVENT_BUF = 255;
	class Event
	{
	public:
		Event(int fd);

		int Read();

		int Write(char * buf, int  len);

		int Close();

		int ReadBuf(char * src, int len);

		int GetErro();

		void SetFdTimeOut();

		void SetFdLinger(int open, int sec);

		int GetFd(){return m_fd;}
	private:
		int		m_fd;
		char	m_buf[EVENT_BUF];
		int		m_write;
		int		m_read;
		int		m_erro;
	};

}

}

#endif
