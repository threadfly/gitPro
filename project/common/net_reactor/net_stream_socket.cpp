#include "net_stream_socket.h"
#include "net_manager.h"
#include "net_packet.h"
#include "log/sync_log.h"
#include "net_share.h"
#include "net_reactor.h"
#include "net_packet_head.h"

namespace Common
{

namespace NetReactor
{

StreamSocket::StreamSocket(NetManager * nm, int fd, string & local_ip, int local_port, string & remote_ip, int remote_port):
EventHandler(nm),
m_local_ip(local_ip),
m_local_port(local_port),
m_remote_ip(remote_ip),
m_remote_port(remote_port)
{
	m_socket.SetSocketId(fd);
}

int StreamSocket::GetFd()
{
	return m_socket.GetSocketId();
}

int StreamSocket::HandleInput()
{
	SyncLog::LOG(INFO, "StreamSocket HandleInput");

	int n = 0;

	NetPacket * ppacket = m_net_manager->GetBuffNetPacket(m_id);
	if ( NULL == ppacket )
	{
		ppacket = new NetPacket(m_id);
		if ( NULL == ppacket )
		{
			SyncLog::LOG(EROR, "StreamSocket HandleInput");
			return -1;
		}

		n = m_socket.Recv((void *)&(ppacket->m_head), NET_PACKET_HEAD_SIZE, 0);
		if ( n != NET_PACKET_HEAD_SIZE)
		{
			SyncLog::LOG(EROR, "StreamSocket HandleInput Recv have enough one PacketHead length");
			return -2;
		}
	}

	/* 这里有一大堆的问题,比方说发过来缓存没有组成一个包长(通过报头知道长度),肿么办?
	 * 又如果超过一个包长,超过的包长我该如何处理,暂时存放在哪,然后等下次凑成一个包长再提交给应用?
	 * 方案一: 先读一个包长的长度,根据包长的信息读取剩下的数据 缺点:读两次
	 */

	// 证明有一个包还没接收完毕
	//TODO 这里没有考虑NetPacket. m_buff 不够大的情况 目前设置为4KB
	int recv_size = (ppacket->m_head.length) - (ppacket->m_buff_len);
	n = m_socket.Recv((void *)(ppacket->m_buff + ppacket->m_buff_len), recv_size , 0);
	if ( n == recv_size)
	{// 一个包接收完毕
		ppacket->m_buff_len += n; 
		m_net_manager->AddRecvNetPacket(ppacket);
	}
	else if ( n > 0)
	{// 证明只接受了一部分
		//XXX 这里有个逻辑bug,如果非法的包过来永远不能被主线程拿到这个NetPackt
		// 而这个NetPacket一直接受到满
		ppacket->m_buff_len += n;
		m_net_manager->SetBuffNetPacket(m_id, ppacket);
	}
	else if ( -1 == n)
	{
		if (EAGAIN == errno || EWOULDBLOCK == errno)
		{
			SyncLog::LOG(EROR, "StreamSocket HandleInput Recv errno == EAGAIN or errno == EWOULDBLOCK");
			//TODO 这里其实应该要继续读的,因为这里证明TCP接收缓存数据还没接收完毕
			m_net_manager->SetBuffNetPacket(m_id, ppacket);
		}
		else
		{
			SyncLog::LOG(EROR, "StreamSocket HandleInput Recv errno:%d",errno);
			delete ppacket;
		}
		return -1;
	}
	else if ( 0 == n )
	{
		SyncLog::LOG(INFO, "StreamSocket HandleInput Recv , Remote Close");
		delete ppacket;
		//m_net_manager->GetReactor()->RemoveHandler(this);
		// TODO 这里异常这个事件后, 怎么销毁它呢? ......
		return -2;
	}

	return n;
}

int StreamSocket::HandleOutput(const void * buff, size_t nbytes)
{
	SyncLog::LOG(INFO, "StreamSocket HandleOutput");
	return m_socket.Send(buff, nbytes, 0);
}

int StreamSocket::HandleClose()
{
	return m_socket.Close();
}

int StreamSocket::Write(const void * buff, size_t nbytes)
{
	return m_socket.Send(buff, nbytes, 0);
}

}

}


