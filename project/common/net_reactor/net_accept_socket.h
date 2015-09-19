#ifndef _NET_ACCEPT_SOCKET
#define _NET_ACCEPT_SOCKET

#include <string>
#include "net_event_handler.h"
#include "net_socket.h"

namespace Common
{

namespace NetReactor
{
	using namespace std;
	class NetManager;

	class AcceptSocket:public EventHandler
	{
	public:
		AcceptSocket(NetManager * );

		virtual ~AcceptSocket(){};

		virtual int GetFd();

		virtual int HandleInput();

		virtual int HandleOutput(const void *, size_t nbytes);

		virtual int HandleClose();

		int CreateTcpServer(const string & ip, const int port );

		void CloseTcpServer();
	private:
		string			m_local_ip;
		int				m_local_port;
		Socket			m_socket;
	};

}

}

#endif

