#ifndef _NET_STREAM_SOCKET_
#define _NET_STREAM_SOCKET_

#include <string>

#include "net_socket.h"
#include "net_event_handler.h"

namespace Common
{

namespace NetReactor
{
	using namespace std;
	class NetManager;
	class Socket;

	class StreamSocket:public EventHandler
	{
	public:
		StreamSocket(NetManager * , int socketfd,  string & local_ip, int local_port, string & remote_ip, int remote_port);

		virtual ~StreamSocket(){};

		//int Init();
		virtual int GetFd();

		virtual int HandleInput();

		virtual int HandleOutput(const void *, size_t nbytes);
		
		virtual int HandleClose();

		int Write(const void * buff, size_t nbytes);

	private:
		string			m_local_ip;
		int				m_local_port;
		string			m_remote_ip;
		int				m_remote_port;
		Socket			m_socket;
	};

}

}

#endif
