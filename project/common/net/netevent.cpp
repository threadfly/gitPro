#include "net.h"
#include "netevent.h"

#define TIMEOUT_SEC 0
#define TIMEOUT_USEC 400000
namespace Common
{

namespace Net
{

	Event::Event(int fd):
	m_fd(fd),
	m_write(0),
	m_read(0),
	m_erro(0)
	{
	}
	
	int Event::ReadBuf(char * src, int len)
	{
		if ( EVENT_BUF <= m_read && EVENT_BUF <= m_write)
		{
			m_read = 0;
			m_write = 0;
			return 0;
		}

		if ( len <= (m_write - m_read))
		{
			bcopy(m_buf+m_read, src, len);
			m_read += len;
			if (m_read >= EVENT_BUF)
			{
				m_read = 0;
				m_write = 0;
			}
			return len;
		}
		else if ( len > (m_write - m_read))
		{
			int read_size = m_write - m_read;
			bcopy(m_buf+m_read, src, read_size);
			m_read = m_write;
			if (m_read >=  EVENT_BUF)
			{
				m_read = 0;
				m_write = 0;
			}
			return read_size;
		}
		return 0;
	}

	int Event::Read()
	{	
		//char *p = m_buf;
		//int len = EVENT_BUF - m_index -1;
		int ret = 0;
		if ( m_write == m_read && m_read == 0)
		{
			ret = recv(m_fd, (void *)(m_buf), EVENT_BUF, 0);
		}
		else if (m_write  < EVENT_BUF)
		{
			ret = recv(m_fd, (void *)(m_buf+m_write), EVENT_BUF - m_write, 0);
		}
		else if (m_write == m_read && m_read >= EVENT_BUF)
		{
			m_erro = EWOULDBLOCK;
			return -1;
		}

		m_erro = errno;
		if ( ret < 0 )
		{
			return -1;
		}
		else if ( ret == 0)
		{
			return 0;
		}
		else 
		{
			m_write += ret;
			return ret;
		}
	}

	int Event::Write(char * buf, int len)
	{
		int ret = send(m_fd, (void*)buf, len , 0);
		m_erro = errno;
		if ( ret < 0 )
		{
			return -1;
		}
		else if ( ret == 0)
		{
			return 0;
		}
		else 
		{
			return ret;
		}
	}

	int Event::Close()
	{
		return ::close(m_fd);
	}

	int Event::GetErro()
	{
		return m_erro;
	}

	void Event::SetFdTimeOut()
	{
		struct timeval tv;
		tv.tv_sec = TIMEOUT_SEC;
		tv.tv_usec = TIMEOUT_USEC;
		setsockopt(m_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	}

	void Event::SetFdLinger(int open, int sec)
	{
		struct linger tmp;
		tmp.l_onoff = open;
		tmp.l_linger = sec;
		int ret = setsockopt(m_fd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
		if (ret != 0)
		{
			perror("Event SetFdLinger Error ");
		}
	}
}

}

