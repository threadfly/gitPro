#include <unistd.h>

#include "net_eventfd.h"
#include "net_manager.h"
#include "log/sync_log.h"
#include "net_packet.h"

namespace Common
{

namespace NetReactor
{

LinuxEventFd::LinuxEventFd(NetManager * nm):
EventHandler(nm)
{
	
}

int LinuxEventFd::Init()
{
	m_efd = eventfd(0, 0);
	if (m_efd == -1 )
	{
		SyncLog::LOG(EROR, "eventfd Error!!");
		return -1;
	}
	return  0;
}

int LinuxEventFd::GetFd()
{
	return m_efd;
}

int LinuxEventFd::HandleInput()
{	
	uint64_t tmp;
	int ret = ::read(m_efd, (void *)&tmp, sizeof(tmp));

	NetPacket * packet = m_net_manager->GetSendNetPacket();
	if (NULL == packet) 
	{
		SyncLog::LOG(EROR, "LinuxEventFd HandleInput Error");
		delete packet;
		return -1;
	}

	EventHandler * phd = m_net_manager->GetHandleById(packet->m_handler_id);
	if ( NULL == phd )
	{
		SyncLog::LOG(EROR, "LinuxEventFd HandleInput phd is NULL");
		delete packet;
		return -1;
	}
	SyncLog::LOG(DEBUG, "paket:%s len:%d", packet->m_buff, packet->m_head.length);
	phd->HandleOutput((void *)&(packet->m_head), sizeof(packet->m_head));
	phd->HandleOutput(packet->m_buff, (packet->m_head).length);
	delete packet;
	return 0;
}

int LinuxEventFd::HandleOutput(const void *, size_t nbytes)
{
	return 0;
}

int LinuxEventFd::Signal()
{	
	//int tmp = 1;
	uint64_t tmp = 1;
	int ret = ::write(m_efd, (void *)&tmp, sizeof(tmp));
	if ( ret == -1)
	{
		SyncLog::LOG(EROR, "LinuxEventFd Signal Write Error, errno:%d", errno);
		return -1;
	}

	return 0;
}

int LinuxEventFd::HandleClose()
{
	return ::close(m_efd);
}

}

}
