#include "net_socket.h"
#include "log/sync_log.h"

namespace Common
{

namespace NetReactor
{

int Socket::Init(int family, int type, int protocol)
{
	m_socketid = ::socket(family, type, protocol);
	if ( m_socketid < 0)
	{
		SyncLog::LOG(EROR, "Socket Init Error, family:%d type:%d, protocol:%d",family, type, protocol);
	}
	return m_socketid;
}

int Socket::Bind(const SA * sa, int len)
{
	if ( NULL == sa || 0 == len )
	{
		SyncLog::LOG(EROR, "Socket Bind Error, sa:%x len:%d", sa, len);
		return -1;
	}

	int ret = ::bind(m_socketid, sa, len);
	if ( -1 == ret )
	{
		SyncLog::LOG(EROR, "Socket Bind Error, ret:%d errno:%d", ret, errno);
	}
	return ret;
}

int Socket::Listen(int lisq)
{
	char * ptr;
	if ( (ptr = ::getenv("LISTENQ")) != NULL)
	{
		lisq = ::atoi(ptr);
	}

	int ret = ::listen(m_socketid, lisq);
	if ( -1 == ret )
	{
		SyncLog::LOG(EROR, "Socket Listen Error, ret:%d errno:%d", ret, errno);
	}

	return ret;
}

int Socket::Accept(SA * sa, SL *sl)
{
	int ret = ::accept(m_socketid, sa, sl);
	if ( -1 == ret )
	{
		SyncLog::LOG(EROR, "Socket Accept Error, ret:%d errno:%d", ret, errno);
	}
	return ret;
}

int Socket::Connect(const SA * sa, int len)
{
	if ( NULL == sa || 0 == len )
	{
		SyncLog::LOG(EROR, "Socket Connect Error, sa:%x len:%d", sa, len);
		return -1;
	}

	int ret = ::connect(m_socketid, sa, len);
	if ( -1 == ret )
	{
		SyncLog::LOG(EROR, "Socket Connect Error, ret:%d errno:%d", ret, errno);
	}
	return ret;
}

int Socket::Recv(void * buff, size_t nbytes, int flags)
{
	if ( NULL == buff || 0 == nbytes)
	{
		SyncLog::LOG(EROR, "Socket Recv Error, buff:%x nbytes:%d", buff, nbytes);
		return -1;
	}

	int n = ::recv(m_socketid, buff, nbytes, flags);
	if ( -1 == n)
	{
		SyncLog::LOG(EROR, "Socket Recv Error, n:%d errno:%d", n, errno);
	}

	return n;
}

int Socket::Send(const void * buff, size_t nbytes, int flags)
{
	if ( NULL == buff || 0 == nbytes)
	{
		SyncLog::LOG(EROR, "Socket Send Error, buff:%x nbytes:%d", buff, nbytes);
		return -1;
	}
	
	int n = ::send(m_socketid, buff, nbytes, flags);
	if ( -1 == n )
	{
		SyncLog::LOG(EROR, "Socket Send Error, n:%d errno:%d", n, errno);
	}

	return n;
}

int Socket::RecvFrom(void * buff, size_t nbytes, int flags, SA * sa, SL * sl)
{
	if ( NULL == buff || 0 ==  nbytes)
	{
		SyncLog::LOG(EROR, "Socket RecvFrom Error, buff:%x nbytes:%d", buff, nbytes);
		return -1;
	}

	int n = ::recvfrom(m_socketid, buff, nbytes, flags, sa, sl);
	if ( -1 == n)
	{
		SyncLog::LOG(EROR, "Socket RecvFrom Error, n:%d errno:%d", n, errno);
	}

	return n;
}

int Socket::SendTo(const void * buff, size_t nbytes, int flags, const SA * sa, SL sl)
{
	if ( NULL == buff || 0 == nbytes || NULL == sa || 0 == sl)
	{
		SyncLog::LOG(EROR, "Socket SendTo Error, buff:%x nbytes:%d sa:%x sl:%d", buff, nbytes, sa, sl);
		return -1;
	}
	
	int n = ::sendto(m_socketid, buff, nbytes, flags, sa, sl);
	if ( -1 == n )
	{
		SyncLog::LOG(EROR, "Socket SendTo Error, n:%d errno:%d", n, errno);
	}

	return n;
}

int Socket::GetSocketOpt(int level, int optname, void * optval, SL * sl)
{
	if ( NULL == optval)
	{
		SyncLog::LOG(EROR, "Socket GetSocketOpt Error, NULL == optval");
		return -1;
	}
	return ::getsockopt(m_socketid, level, optname, optval, sl);
}

int Socket::SetSocketOpt(int level, int optname, const void * optval, SL sl)
{
	if ( NULL == optval)
	{
		SyncLog::LOG(EROR, "Socket SetSocketOpt Error, NULL == optval");
		return -1;
	}
	return ::setsockopt(m_socketid, level, optname, optval, sl);
}

}

}
