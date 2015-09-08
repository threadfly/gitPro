#include "netmanager.h"

NetManager::NetManager()
{
#ifdef EPOLL_REACTOR
	m_reactor = new Epoll(this); // TODO  这样做对否, 在NetManager还没完全构造完毕时就把它作为参数去构造另一个?
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
		// TODO  这里怎样能让主线程知道网络线程失败了?
	}
	
	//1. 注册监听端口
	m_reactor->RegisterHandler(papsk);

	while(true)
	{
		//2. 事件循环 (包括等待产生事件, 并处理这些事件)
		m_reactor->RunReactorEventLoop();
		//3. 处理 m_send_packet_deque 队列
		//
		//4. ......
	}
}

int NetManager::Work()
{
	//1. NetManager 线程启动前的加载配置
	LoadConfig();
	//2. 设置
	//
	//3. 启动NetManager线程
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

