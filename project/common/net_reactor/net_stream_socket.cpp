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

	/* ������һ��ѵ�����,�ȷ�˵����������û�����һ������(ͨ����ͷ֪������),��ô��?
	 * ���������һ������,�����İ����Ҹ���δ���,��ʱ�������,Ȼ����´δճ�һ���������ύ��Ӧ��?
	 * ...... 
	 */
	int n = m_socket.Recv((void *)(ppacket->m_buff), NET_PACKET_BUFF_SIZE, 0);
	if ( -1 == n)
	{
		if (EAGAIN == errno || EWOULDBLOCK == errno)
		{
			SyncLog::Log(EROR, "StreamSocket HandleInput Recv errno == EAGAIN or errno == EWOULDBLOCK");
			//TODO ������ʵӦ��Ҫ��������,��Ϊ����֤��TCP���ջ������ݻ�û�������
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
		// TODO �����쳣����¼���, ��ô��������? ......
		return -1;
	}

	ppacket->m_buff_len = n; // TODO Ŀǰ������, ��һ���Ƕ��һ��

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


