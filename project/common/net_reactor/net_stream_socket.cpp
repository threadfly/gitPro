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

	/* ������һ��ѵ�����,�ȷ�˵����������û�����һ������(ͨ����ͷ֪������),��ô��?
	 * ���������һ������,�����İ����Ҹ���δ���,��ʱ�������,Ȼ����´δճ�һ���������ύ��Ӧ��?
	 * ����һ: �ȶ�һ�������ĳ���,���ݰ�������Ϣ��ȡʣ�µ����� ȱ��:������
	 */

	// ֤����һ������û�������
	//TODO ����û�п���NetPacket. m_buff ���������� Ŀǰ����Ϊ4KB
	int recv_size = (ppacket->m_head.length) - (ppacket->m_buff_len);
	n = m_socket.Recv((void *)(ppacket->m_buff + ppacket->m_buff_len), recv_size , 0);
	if ( n == recv_size)
	{// һ�����������
		ppacket->m_buff_len += n; 
		m_net_manager->AddRecvNetPacket(ppacket);
	}
	else if ( n > 0)
	{// ֤��ֻ������һ����
		//XXX �����и��߼�bug,����Ƿ��İ�������Զ���ܱ����߳��õ����NetPackt
		// �����NetPacketһֱ���ܵ���
		ppacket->m_buff_len += n;
		m_net_manager->SetBuffNetPacket(m_id, ppacket);
	}
	else if ( -1 == n)
	{
		if (EAGAIN == errno || EWOULDBLOCK == errno)
		{
			SyncLog::LOG(EROR, "StreamSocket HandleInput Recv errno == EAGAIN or errno == EWOULDBLOCK");
			//TODO ������ʵӦ��Ҫ��������,��Ϊ����֤��TCP���ջ������ݻ�û�������
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
		// TODO �����쳣����¼���, ��ô��������? ......
		return -2;
	}

	return n;
}

int StreamSocket::HandleOutput(const void * buff, size_t nbytes)
{
	SyncLog::LOG(INFO, "StreamSocket HandleOutput");
	return 0;
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


