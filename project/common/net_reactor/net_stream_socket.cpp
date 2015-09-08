#include "net_stream_socket.h"

namespace Common
{

namespace NetReactor
{

StreamSocket::StreamSocket(NetManager * nm, string & local_ip, int local_port, string & remote_ip, int remote_port):
m_net_manager(nm),
m_local_ip(local_ip),
m_local_port(local_port),
m_remote_ip(remote_ip),
m_remote_port(remote_port),
{
}

int StreamSocket::GetFd()
{
	return m_socket.GetSocketId();
}

int StreamSocket::HandleInput()
{
	NetPacket * ppacket = New NetPacket();
	if ( NULL == ppacket )
	{
		SyncLog::Log(EROR, "StreamSocket HandleInput");
		return -1;
	}

	/* 这里有一大堆的问题,比方说发过来缓存没有组成一个包长(通过报头知道长度),肿么办?
	 * 又如果超过一个包长,超过的包长我该如何处理,暂时存放在哪,然后等下次凑成一个包长再提交给应用?
	 * ...... 
	 */
	int n = m_socket.Recv((void *)(ppacket->m_buff), NET_PACKET_BUFF_SIZE, 0);
	if ( -1 == n)
	{
		if (EAGAIN == errno || EWOULDBLOCK == errno)
		{
			SyncLog::Log(EROR, "StreamSocket HandleInput Recv errno == EAGAIN or errno == EWOULDBLOCK");
			//TODO 这里其实应该要继续读的,因为这里证明TCP接收缓存数据还没接收完毕
		}
		else
		{
			SyncLog::Log(EROR, "StreamSocket HandleInput Recv errno:%d",errno);
		}
	}
	else if ( 0 == n )
	{
		SyncLog::Log(INFO, "StreamSocket HandleInput Recv , Remote Close");
		m_reactor->RemoveHandler(this);
		// TODO 这里异常这个事件后, 怎么销毁它呢? ......
		return -1;
	}

	ppacket->m_buff_len = n; // TODO 目前的做法, 这一步是多此一举

	m_net_manager.AddRecvNetPacket(ppacket);
}

int StreamSocket::HandleOutput()
{
	SyncLog::Log(INFO, "StreamSocket HandleOutput");
	return 0;
}

int StreamSocket::HandleClose()
{
	m_socket.Close();
}

}

}


