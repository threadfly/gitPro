#include "netmanager.h"

NetManager::NetManager()
{
#ifdef EPOLL_REACTOR
	m_reactor = new Epoll(this); // TODO  �������Է�, ��NetManager��û��ȫ�������ʱ�Ͱ�����Ϊ����ȥ������һ��?
#elif

#endif
}

void NetManager::Run()
{
	AcceptSocket * papsk = new AcceptSocket();
	int ret = papsk->CreateTcpServer(m_listen_ip, m_listen_port);

	if ( -1 == ret)
	{
		SyncLog::Log(EROR, "NetManager Run CreateTcpServer Error, %s:%d", m_listen_ip, m_listen_port);
		return;
		// TODO  ���������������߳�֪�������߳�ʧ����?
	}
	
	//1. ע������˿�
	m_reactor->RegisterHandler(papsk);

	while(true)
	{
		//2. �¼�ѭ�� (�����ȴ������¼�, ��������Щ�¼�)
		m_reactor->RunReactorEventLoop();
		//3. ���� m_send_packet_deque ����
		//
		//4. ......
	}
}

int NetManager::Work()
{
	//1. NetManager �߳�����ǰ�ļ�������
	LoadConfig();
	//2. ����
	//
	//3. ����NetManager�߳�
	if (0 != Start())
		return -1;
	//4. ......
	//
	//5. ......
}

int NetManager::LoadConfig()
{
	m_listen_ip = string("192.168.180.35");

	m_listen_port = 5888;
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
	
	m_send_packet_deque.push_back(packet);

	GetMutex().UnLock();
}

int NetManager::addRecvNetPacket(NetPacket * packet)
{
	GetMutex().Lock();

	m_recv_packet_deque.push_back(packet);

	GetMutex().UnLock();
}

NetPacket * NetManager::getSendNetPacket()
{
	NetPacket * packet = NULL;

	GetMutex().Lock();

	if ( !m_send_packet_deque.empty())
	{
		packet = m_send_packet_deque.front();
		m_send_packet_deque.pop();
	}

	GetMutex().UnLock();
}

