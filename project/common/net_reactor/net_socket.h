#ifndef _NET_SOCKET_
#define _NET_SOCKET_

#include "net_share.h"

namespace Common
{

namespace NetReactor
{
	using namespace Common::Log;
	class Socket
	{
	public:
		int Init(int family, int type, int protocol);

		int Bind(const SA * sa , int len);
	
		int Listen(int lisq = ListenQ);

		int Accept(SA * sa, SL * sl);

		int Connect(const SA * sa, int len);

		int Close(){ return ::close(m_socketid);}

		int GetSocketOpt(int level, int optname, void * optval, SL * sl);

		int SetSocketOpt(int level, int optname, const void * optval, SL sl);
	public:
		/* read & write */
		int Recv(void * buff, size_t nbytes, int flags);

		int Send(const void * buff, size_t nbytes, int flags);

		int RecvFrom(void * buff, size_t nbytes, int flags, SA * sa, SL *sl);

		int SendTo(const void * buff, size_t nbytes,  int flags, const SA * sa, SL sl);

	public:
		int GetSocketId(){ return m_socketid; };

		void SetSocketId(int fd) { m_socketid = fd; }
	private:
		int					m_socketid;
		//struct sockaddr_in	m_servaddr;
	};

}

}

#endif
