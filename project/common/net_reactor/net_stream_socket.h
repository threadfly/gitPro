#ifndef _NET_STREAM_SOCKET_
#define _NET_STREAM_SOCKET_

#include "net_share.h"
#include "net_socket.h"
#include "net_manager.h"
#include "net_packet.h"
#include "log/sync_log.h"

namespace Common
{

namespace NetReactor
{

	class StreamSocket:public EventHandler
	{
	public:
		StreamSocket(NetManager * , string & local_ip, int local_port, string & remote_ip, int remote_port);

		virtual ~StreamSocket(){};

		//int Init();
		virtual int GetFd();

		virtual int HandleInput();

		virtual int HandleOutput();
		
		virtual int HandleClose();
	private:
		string			m_local_ip;
		int				m_local_port;
		string			m_remote_ip;
		string			m_remote_port;
		Socket			m_socket;
	};

}

}

#endif
