#include "net_send_pipe.h"
#include "net_packet_head.h"
#include "net_packet.h"

namespace Common
{

namespace NetReactor
{

SendPipe::SendPipe(NetManager * nm):
EventHandler(nm)
{
	//m_pipe.Init();	//构造函数中做这些操作肯定是有问题的
}

int SendPipe::GetFd()
{
	return m_pipe.m_pipefd[0];
}

int SendPipe::HandleInput()
{
	InnerPacketHead head;
	::memset(&head, 0, sizeof(head));
	int n = m_pipe.Read((void *)&head, sizeof(head));
	if ( n != sizeof(head))
	{
		SyncLog::LOG(EROR, "SendPipe HandleInput there is error, n != sizeof(head)");
		return -1;
	}
	
	NetPacket packet(0);
	NetPacketHead nhead;

	nhead.length = head.length;
	nhead.sid = head.sid;
	nhead.uid = head.uid;
	nhead.cmd = head.cmd;

	n = m_pipe.Read((void *)packet.m_buff, nhead.length);
	if ( n != (int)nhead.length)
	{
		SyncLog::LOG(EROR, "SendPipe HandleInput Read , n != nhead.length, n:%d", n);
		return -1;
	}
	
	EventHandler * phd = m_net_manager->GetHandleById(head.hid);
	if ( NULL == phd )
	{
		SyncLog::LOG(EROR, "SendPipe phd is NULL");
		return -1;
	}
	phd->HandleOutput((void *)&nhead, sizeof(nhead));
	phd->HandleOutput(packet.m_buff, nhead.length);
	
	return 0;
}

int SendPipe::HandleOutput(const void * buff, size_t nbytes)
{
	return m_pipe.Write(buff, nbytes);
}

int SendPipe::HandleClose()
{
	m_pipe.CloseRead();
	m_pipe.CloseWrite();
	return 0;
}

int SendPipe::Init(int flags)
{
	return m_pipe.Init(flags);
}
/*
int Write(const void * buff, uint32_t nbytes)
{
	return m_pipe.Write(buff, nbytes);
}*/

}

}

