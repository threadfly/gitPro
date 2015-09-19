#ifndef _NET_CONNECT_SOCKET
#define _NET_CONNECT_SOCKET

#include <string>

#include "net_event_handler.h"
#include "net_socket.h"

namespace Common
{

namespace NetReactor
{
	using namespace std;
	class NetManager;

	class ConnectSocket: public EventHandler
	{
	public:
		ConnectSocket(NetManager * );

		virtual ~ConnectSocket(){};

		virtual int GetFd();

		virtual int HandleInput();

		virtual int HandleOutput(const void *, size_t nbytes);

		virtual int HandleClose();

		int CreateTcpClient(const string & ip, const int port);

		void CloseTcpClient();
	private:
		Socket			m_socket;
		string			m_remote_ip;
		int				m_remote_port;
	};

}

}

#endif
