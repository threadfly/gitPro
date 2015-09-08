#ifndef _NET_CONNECT_SOCKET
#define _NET_CONNECT_SOCKET

#include "net_share.h"
#include "net_event_handler.h"
#include "net_socket.h"
#include "net_manager.h"
#include "log/sync_log.h"

namespace Common
{

namespace NetReactor
{

	class ConnectSocket: public EventHandler
	{
	public:
		ConnectSocket(NetManager * );

		virtual ~ConnectSocket(){};

		virtual int GetFd();

		virtual int HandleInput();

		virtual int HandleOutput();

		virtual int HandleClose();

		int CreateTcpClient(const string & ip, const int port);

		void CloseTcpClient();
	private:
		NetManager *	m_net_manager;
		string			m_remote_ip;
		int				m_remote_port;
	};

}

}

#endif
