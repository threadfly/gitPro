#include "net_manager.h"
#include "net_packet.h"
#include "net_reactor.h"
#include "net_share.h"
#include "net_epoll.h"
#include "net_accept_socket.h"
#include "net_stream_socket.h"
#include "net_packet_head.h"
#include "net_send_pipe.h"
#include "tiny_xml/tinyxml.h"
#include "net_eventfd.h"

namespace Common
{

namespace NetReactor
{

NetManager::NetManager():
Thread(true),
m_stop(false),
m_send_pipe(NULL)
//m_id_gnt()
{
#ifdef EPOLL_REACTOR
	m_reactor = new Epoll(this); // TODO  这样做对否, 在NetManager还没完全构造完毕时就把它作为参数去构造另一个?
//#elif

#endif
}

void NetManager::Run()
{

	while(true)
	{
		//2. 事件循环 (包括等待产生事件, 并处理这些事件)
		m_reactor->RunReactorEventLoop();
		//3. 处理 m_send_packet_deque 队列
		//SendPacketDeque();
		//4. ......
		if ( m_stop)
		{
			break;
		}
	}
	// 关掉时处理未发送的...
	ID_HANDLER_MAP::iterator it; 
	for( ; it != m_handler_idm.end(); ++it)
	{
		m_reactor->RemoveHandler(it->second);
		it->second->HandleClose();
		delete it->second;
	}
}

int NetManager::Work(const char * configfile)
{
	//1. NetManager 线程启动前的加载配置
	if ( -1 == LoadConfig(configfile))
	{
		SyncLog::LOG(EROR, "NetManager LoadConfig Error");
		return -1;
	}
	//2. 设置
	AcceptSocket * papsk = new AcceptSocket(this);

	int ret = papsk->CreateTcpServer(m_listen_ip, m_listen_port);

	if ( -1 == ret)
	{
		SyncLog::LOG(EROR, "NetManager Work CreateTcpServer Error, %s:%d", m_listen_ip.c_str(), m_listen_port);
		return -1;
		// TODO  这里怎样能让主线程知道网络线程失败了? A:放这里不就解决啦!!
	}
	
	//3. 注册监听端口
	if ( -1 ==  m_reactor->RegisterHandler(papsk))
	{
		SyncLog::LOG(EROR, "NetManager Work RegisterHandler Error, %s:%d", m_listen_ip.c_str(), m_listen_port);
		return -1;
	}

	//4. 注册发送管道描述符到Reactor上
	m_send_pipe = new SendPipe(this);
	ret = m_send_pipe->Init();
	if (-1 == ret)
	{
		SyncLog::LOG(EROR, "NetManager Work m_send_pipe Init Error");
		return -1;
	}

	if ( -1 == m_reactor->RegisterHandler(m_send_pipe))
	{
		SyncLog::LOG(EROR, "NetManager Work m_send_pipe RegisterHandler Error");
		return -1;
	}
	// 注册eventfd 用来通知写事件 //TODO 以后放到一个函数中
	m_wfd = new LinuxEventFd(this);
	ret = m_wfd->Init();
	if (-1 == ret)
	{
		SyncLog::LOG(EROR, "NetManager Work m_wfd Init Error");
		return -1;
	}

	if ( -1 == m_reactor->RegisterHandler(m_wfd))
	{
		SyncLog::LOG(EROR, "NetManager Work m_wfd RegisterHandler Error");
		return -1;
	}
	//5. 启动NetManager线程
	if (0 != Start())
		return -1;
	//6. ......
	//
	//7. ......
	return 0;
}

void NetManager::Wait()
{
	GetMutex().Lock();

	GetCond().Wait(GetMutex());

	GetMutex().UnLock();
}

int NetManager::LoadConfig(const char * configfile)
{
	//m_listen_ip = string("192.168.0.108");

	//m_listen_port = 5888;
	// reference: http://www.grinninglizard.com/tinyxmldocs/tutorial0.html
	TiXmlDocument doc(configfile);
	if (!doc.LoadFile())
	{
		SyncLog::LOG(EROR, "NetManager LoadConfig Error, configfile:%s", configfile);
		return -1;
	}

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);

	pElem = hDoc.FirstChildElement().Element();
	if (!pElem)
	{
		SyncLog::LOG(EROR, "NetManager LoadConfig FirstChildElement Error, pElem:NULL");
		return -1;
	}

	hRoot = TiXmlHandle(pElem);

	pElem = hRoot.FirstChild("server").Element();
	if (pElem)
	{
		m_listen_ip = pElem->Attribute("ip");
		pElem->QueryUnsignedAttribute("port", &m_listen_port);
	}
	else
	{
		SyncLog::LOG(EROR, "NetManage LoadConfig FirstChld Error, pElem:NULL");
		return -1;
	}
	/*for ( pElem; pElem != NULL ; pElem=pElem->NextSiblingElement())
	{
		const char * pKey = pElem->Value();
		SyncLog::LOG(INFO, "NetManager LoadConfig, pKey:%s", pKey);
	}*/

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

	GetCond().Signal();

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
/*
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
			SyncLog::LOG(EROR, "NetManager SendPacketDeque hid have Deleted!!%s",packet->m_buff);
		}
		else
		{
			StreamSocket * pss = dynamic_cast<StreamSocket *>(it->second);
			if ( NULL != pss)
			{
				pss->Write(packet->m_buff, ::strlen(packet->m_buff));
			}
		}

		delete packet;
		packet = NULL;
		packet = GetSendNetPacket();
	}

}*/

uint32_t  NetManager::GenerateId()
{
	return m_id_gnt.GetNewID();
}

bool NetManager::SetHandlerId(uint32_t id, EventHandler * eh)
{
	if ( m_handler_idm.insert(std::pair<uint32_t , EventHandler *>(id, eh)).second)
		return true;
	return false;
}

void NetManager::RemoveHandlerId(uint32_t id)
{
	m_handler_idm.erase(id);
}

EventHandler * NetManager::GetHandleById(uint32_t id)
{
	ID_HANDLER_MAP::iterator it = m_handler_idm.find(id);
	if (it != m_handler_idm.end())
	{
		return it->second;
	}
	return NULL;
}


void NetManager::Stop()
{
	m_stop = true;
}

NetPacket * NetManager::GetBuffNetPacket(uint32_t hid)
{
	NetPacket * tmp = NULL;
	ID_PACKET_MAP::iterator it =  m_buff_packet_idm.find(hid);
	if ( it != m_buff_packet_idm.end())
	{
		tmp = it->second;
		m_buff_packet_idm.erase(it);
		return tmp;
	}
	return NULL;
}

bool NetManager::SetBuffNetPacket(uint32_t hid, NetPacket * packet)
{
	if ( m_buff_packet_idm.insert(std::pair<uint32_t, NetPacket *>(hid, packet)).second)
	{
		return true;
	}

	return false;
}

void NetManager::SendMessage(InnerPacketHead &head,const void * message)
{
	int length = head.length;
	m_send_pipe->HandleOutput((void *)&head, sizeof(head));
	m_send_pipe->HandleOutput(message, length);
	//WriteSendPipe((void *)&head, sizof(head));
	//WriteSendPipe(message, length);
}

int NetManager::WorkAsServer(const char * configfile)
{
	return Work(configfile);
}

int NetManager::WorkAsClient()
{
	//4. 注册发送管道描述符到Reactor上
	m_send_pipe = new SendPipe(this);
	int ret = m_send_pipe->Init();
	if (-1 == ret)
	{
		SyncLog::LOG(EROR, "NetManager Work m_send_pipe Init Error");
		return -1;
	}

	if ( -1 == m_reactor->RegisterHandler(m_send_pipe))
	{
		SyncLog::LOG(EROR, "NetManager Work m_send_pipe RegisterHandler Error");
		return -1;
	}
	// 注册eventfd 用来通知写事件
	m_wfd = new LinuxEventFd(this);
	ret = m_wfd->Init();
	if (-1 == ret)
	{
		SyncLog::LOG(EROR, "NetManager Work m_wfd Init Error");
		return -1;
	}

	if ( -1 == m_reactor->RegisterHandler(m_wfd))
	{
		SyncLog::LOG(EROR, "NetManager Work m_wfd RegisterHandler Error");
		return -1;
	}

	//5. 启动NetManager线程
	if (0 != Start())
		return -1;
	//6. ......
	
	return 0;
}

int NetManager::SendMessageByEvtFd(NetPacket * packet)
{
	if (packet == NULL )
	{
		SyncLog::LOG(EROR, "SendMessageByEvtFd packet is NULL");
		return -1;
	}

	AddSendNetPacket(packet);

	m_wfd->Signal();

	return 0;
}

}

}
