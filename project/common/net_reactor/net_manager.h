#ifndef _NET_MANAGER_
#define _NET_MANAGER_

#include <queue>
#include <string>
#include <map>
#include <stdint.h>

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
	class InnerPacketHead;
	class SendPipe;
	class LinuxEventFd;

	class NetManager:public Common::Thread::Thread
	{
	public:
		typedef std::map<uint32_t, EventHandler*> ID_HANDLER_MAP;

		typedef std::map<uint32_t, NetPacket *> ID_PACKET_MAP;
	public:
		NetManager();

		virtual ~NetManager(){};

		virtual void Run();

		int Work(const char * configfile);
		
		int WorkAsServer(const char * configfile);

		int WorkAsClient();

		void Stop();
		//һ�����������̵߳ȴ��������߳̽��յ��ͻ���������
		void Wait();

		int LoadConfig(const char * configfile);

		NetPacket * GetRecvNetPacket();

		int AddRecvNetPacket(NetPacket *);

		int AddSendNetPacket(NetPacket *);
		
		Reactor * GetReactor(){ return m_reactor;}
	
		uint32_t GenerateId();

		bool SetHandlerId(uint32_t, EventHandler * );

		void RemoveHandlerId(uint32_t);

		EventHandler * GetHandleById(uint32_t);
		
		void SendMessage(InnerPacketHead &head,const void * message);

		NetPacket * GetBuffNetPacket(uint32_t);

		bool SetBuffNetPacket(uint32_t, NetPacket *);

	public:
		int SendMessageByEvtFd( NetPacket *);

		NetPacket * GetSendNetPacket();
	private:
		//NetPacket * GetSendNetPacket();

		//void SendPacketDeque();

		//void WriteSendPipe(const void *, uint32_t );
	private:
		std::queue<NetPacket *> m_recv_packet_deque;

		std::queue<NetPacket *> m_send_packet_deque;

		Reactor *				m_reactor;

		// ����Ӧ���Ǵ������м��ؽ�����
		string					m_listen_ip;
		
		uint32_t				m_listen_port;

		IDGenerator				m_id_gnt;

		ID_HANDLER_MAP			m_handler_idm;
		
		// ���Ӧ����δ��ȡ��һ������ʱ���,�¶���������������,ֱ����һ����
		// �ٷ���m_recv_packet_deque��
		ID_PACKET_MAP			m_buff_packet_idm;

		bool					m_stop;

		SendPipe *				m_send_pipe;
		
		// ֪ͨд�¼� TODO �Ժ�ָ�볢��������ָ���滻
		LinuxEventFd *			m_wfd;
	};
}

}


#endif

