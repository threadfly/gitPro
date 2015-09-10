#include "net_manager.h"
#include "net_packet.h"
#include "net_reactor.h"
#include "net_share.h"
#include "net_epoll.h"
#include "net_accept_socket.h"
#include "net_stream_socket.h"

namespace Common
{

namespace NetReactor
{

NetManager::NetManager():
Thread(false),
m_id_gnt()
{
#ifdef EPOLL_REACTOR
	m_reactor = new Epoll(this); // TODO  �������Է�, ��NetManager��û��ȫ�������ʱ�Ͱ�����Ϊ����ȥ������һ��?
//#elif

#endif
}

void NetManager::Run()
{

	while(true)
	{
		//2. �¼�ѭ�� (�����ȴ������¼�, ��������Щ�¼�)
		m_reactor->RunReactorEventLoop();
		//3. ���� m_send_packet_deque ����
		SendPacketDeque();
		//4. ......
	}
}

int NetManager::Work()
{
	//1. NetManager �߳�����ǰ�ļ�������
	LoadConfig();
	//2. ����
	AcceptSocket * papsk = new AcceptSocket(this);

	int ret = papsk->CreateTcpServer(m_listen_ip, m_listen_port);

	if ( -1 == ret)
	{
		SyncLog::LOG(EROR, "NetManager Work CreateTcpServer Error, %s:%d", m_listen_ip.c_str(), m_listen_port);
		return -1;
		// TODO  ���������������߳�֪�������߳�ʧ����? A:�����ﲻ�ͽ����!!
	}
	
	//3. ע������˿�
	if ( -1 ==  m_reactor->RegisterHandler(papsk))
	{
		SyncLog::LOG(EROR, "NetManager Work RegisterHandler Error, %s:%d", m_listen_ip.c_str(), m_listen_port);
		return -1;
	}
	//4. ����NetManager�߳�
	if (0 != Start())
		return -1;
	//4. ......
	//
	//5. ......
	return 0;
}

int NetManager::LoadConfig()
{
	m_listen_ip = string("192.168.180.35");

	m_listen_port = 5888;

	return 0;
}

NetPacket * NetManager::GetRecvNetPacket()
{
	NetPacket * packet = NULL;
	GetMutex().Lock();

	if ( !m_recv_packet_deque.empty())
	{
		packet = m_recv_packet_deque.front();
		m_recv_packet_deque.pop();
	}

	GetMutex().UnLock();

	return packet;
}

int NetManager::AddSendNetPacket(NetPacket * packet)
{
	GetMutex().Lock();
	
	m_send_packet_deque.push(packet);

	GetMutex().UnLock();

	return 0;
}

int NetManager::AddRecvNetPacket(NetPacket * packet)
{
	GetMutex().Lock();

	m_recv_packet_deque.push(packet);

	GetMutex().UnLock();

	return 0;
}

NetPacket * NetManager::GetSendNetPacket()
{
	NetPacket * packet = NULL;

	GetMutex().Lock();

	if ( !m_send_packet_deque.empty())
	{
		packet = m_send_packet_deque.front();
		m_send_packet_deque.pop();
	}

	GetMutex().UnLock();
	return packet;
}

void NetManager::SendPacketDeque()
{
	int limit = 50;
	NetPacket * packet = GetSendNetPacket();

	while( NULL != packet && limit != 0)
	{
		--limit;
		ID_HANDLER_MAP::iterator it = m_handler_idm.find(packet->m_handler_id);
		if (it == m_handler_idm.end())
		{
			SyncLog::LOG(EROR, "NetManager SendPacketDeque hid have Deleted!!");
			continue;
		}

		StreamSocket * pss = dynamic_cast<StreamSocket *>(it->second);
		if ( NULL != pss)
		{
			pss->Write(packet->m_buff, sizeof(packet->m_buff));
		}

		delete packet;
		packet = NULL;
		packet = GetSendNetPacket();
	}

}

long NetManager::GenerateId()
{
	return m_id_gnt.GetNewID();
}

bool NetManager::SetHandlerId(long id, EventHandler * eh)
{
	if ( m_handler_idm.insert(std::pair<long, EventHandler *>(id, eh)).second)
		return true;
	return false;
}

void NetManager::RemoveHandlerId(long id)
{
	m_handler_idm.erase(id);
}

}

}
