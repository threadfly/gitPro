#ifndef _NET_MANAGER_
#define _NET_MANAGER_

#include <queue>
#include <string>
#include <map>

#include "thread/thread.h"
#include "idgenerator.h"

namespace Common
{

namespace NetReactor
{
	using namespace std;

	using namespace Common::Thread;
	using namespace Common::Tools;

	class Reactor;
	class NetPacket;
	class EventHandler;

	class NetManager:public Common::Thread::Thread
	{
	public:
		typedef std::map<long, EventHandler*> ID_HANDLER_MAP;

		typedef std::map<uint32_t, >
	public:
		NetManager();

		virtual ~NetManager(){};

		virtual void Run();

		int Work();

		void Stop();
		//一般用于其他线程等待此网络线程接收到客户端数据用
		void Wait();

		int LoadConfig();

		NetPacket * GetRecvNetPacket();

		int AddRecvNetPacket(NetPacket *);

		int AddSendNetPacket(NetPacket *);
		
		Reactor * GetReactor(){ return m_reactor;}
	
		long GenerateId();

		bool SetHandlerId(long, EventHandler * );

		void RemoveHandlerId(long );
	private:
		NetPacket * GetSendNetPacket();

		void SendPacketDeque();

	private:
		std::queue<NetPacket *> m_recv_packet_deque;

		std::queue<NetPacket *> m_send_packet_deque;

		Reactor *				m_reactor;

		// 这里应该是从配置中加载进来的
		string					m_listen_ip;
		
		int						m_listen_port;

		IDGenerator				m_id_gnt;

		ID_HANDLER_MAP			m_handler_idm;

		bool					m_stop;
	};
}

}


#endif

