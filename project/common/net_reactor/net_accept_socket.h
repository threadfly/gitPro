#ifndef _NET_ACCEPT_SOCKET
#define _NET_ACCEPT_SOCKET

#include "net_share.h"
#include "net_event_handler.h"
#include "net_socket.h"
#include "net_manager.h"
#include "log/sync_log.h"

namespace Common
{

namespace NetReactor
{
	
	class AcceptSocket:public EventHandler
	{
	public:
		AcceptSocket(NetManager * );

		virtual ~AcceptSocket(){};

		virtual int GetFd();

		virtual int HandleInput();

		virtual int HandleOutput();

		virtual int HandleClose();

		int CreateTcpServer(const string & ip, const int port );

		void CloseTcpServer();
	private:
		NetManager *	m_net_manager;
		string			m_local_ip;
		int				m_local_port;
		Socket			m_socket;
	};

}

}

#endif

