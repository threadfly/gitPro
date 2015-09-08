#ifndef _NET_MANAGER_
#define _NET_MANAGER_

#include <queue>

#include "net_share.h"
#include "net_packet.h"
#include "thread/thread.h"

namespace Common
{

namespace Net
{
	using namespace std;

	using namespace Common::Thread;

	class NetManager::public Thread
	{
	public:
		NetManager();

		virtual ~NetManager(){};

		virtual void Run();

		int Work();

		int LoadConfig();

		NetPacket * GetRecvNetPacket();

		int AddRecvNetPacket(NetPacket *);

		NetPacket * getSendNetPacket();

		int AddSendNetPacket(NetPacket *);
		
		Reactor * GetReactor(){ return m_reactor;};
	private:
		std::queue<NetPacket *> m_recv_packet_deque;

		std::queue<NetPacket *> m_send_packet_deque;

		Reactor *				m_reactor;

		// 这里应该是从配置中加载进来的
		string					m_listen_ip;
		
		int						m_listen_port;
	};
}

}


#endif
