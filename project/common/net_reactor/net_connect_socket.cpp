#include "net_connect_socket.h"
#include "net_manager.h"
#include "log/sync_log.h"
#include "net_share.h"
#include "net_reactor.h"
#include "net_packet.h"
#include "net_packet_head.h"

namespace Common
{

namespace NetReactor
{

ConnectSocket::ConnectSocket(NetManager * nm):
EventHandler(nm)
{
}

int ConnectSocket::GetFd()
{
	return m_socket.GetSocketId();
}

int ConnectSocket::HandleInput()
{
	SyncLog::LOG(INFO, "Client ConnectSocket HandleInput");
	// ��һƬֵCopy From net_stream_socket.cpp
	int n = 0;

	NetPacket * ppacket = m_net_manager->GetBuffNetPacket(m_id);
	if ( NULL == ppacket )
	{
		ppacket = new NetPacket(m_id);
		if ( NULL == ppacket )
		{
			SyncLog::LOG(EROR, "ConnectSocketStreamSocket HandleInput");
			return -1;
		}

		n = m_socket.Recv((void *)&(ppacket->m_head), NET_PACKET_HEAD_SIZE, 0);
		if ( n != NET_PACKET_HEAD_SIZE)
		{
			SyncLog::LOG(EROR, "ConnectSocket HandleInput Recv have enough one PacketHead length");
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
			SyncLog::LOG(EROR, "ConnectSocket HandleInput Recv errno == EAGAIN or errno == EWOULDBLOCK");
			//TODO ������ʵӦ��Ҫ��������,��Ϊ����֤��TCP���ջ������ݻ�û�������
			m_net_manager->SetBuffNetPacket(m_id, ppacket);
		}
		else
		{
			SyncLog::LOG(EROR, "ConnectSocket HandleInput Recv errno:%d",errno);
			delete ppacket;
		}
		return -1;
	}
	else if ( 0 == n )
	{
		SyncLog::LOG(INFO, "ConnectSocket HandleInput Recv , Remote Close");
		delete ppacket;
		//m_net_manager->GetReactor()->RemoveHandler(this);
		// TODO �����쳣����¼���, ��ô��������? ......
		return -2;
	}

	return n;
}

int ConnectSocket::HandleOutput(const void * buff, size_t nbytes)
{
	return m_socket.Send(buff, nbytes, 0);
	//SyncLog::LOG(INFO, "Client ConnectSocket HandleOutput");
	//return 0;
}

int ConnectSocket::CreateTcpClient(const string & ip, const int port)
{
	int ret = -1;
	ret = m_socket.Init(AF_INET, SOCK_STREAM, 0);
	if ( -1 == ret )
	{
		SyncLog::LOG(EROR, "ConnectSocket CreateTcpClient Error, ret:%d", ret);
		return ret;
	}

	m_remote_ip = ip;
	m_remote_port = port;

	SAI sai;
	::memset(&sai, 0, sizeof(sai));
	sai.sin_family = AF_INET;
	if ( m_remote_ip != "")
	{
		if ( 0 ==  inet_pton(AF_INET, m_remote_ip.c_str(), &sai.sin_addr))
		{
			SyncLog::LOG(EROR, "ConnectSocket CreateTcpClient inet_pton error, m_remote_ip:%s", m_remote_ip.c_str());
			return -1;
		}
	}
	else
	{
		//TODO �ͻ��˿��Բ�ָ��Զ��IPô?
		SyncLog::LOG(EROR, "ConnectSocket CreateTcpClient  , m_remote_ip:%s", m_remote_ip.c_str());
		return -1;
	}
	sai.sin_port = htons(m_remote_port);

	ret = m_socket.Connect((SA *)&sai, sizeof(sai));
	if ( -1 ==  ret )
	{
		if (errno == EAGAIN)
		{
			SyncLog::LOG(EROR, "ConnectSocket CreateTcpClient ,errno:EAGAIN  m_remote_ip:%s", m_remote_ip.c_str());
		}
		else if (errno == EWOULDBLOCK)
		{
			SyncLog::LOG(EROR, "ConnectSocket CreateTcpClient ,errno:EWOULDBLOCK m_remote_ip:%s", m_remote_ip.c_str());
		}
		else if (errno == EFAULT)
		{
			SyncLog::LOG(EROR, "ConnectSocket CreateTcpClient ,errno:EFAULT m_remote_ip:%s", m_remote_ip.c_str());
		}
		else if (errno == ECONNABORTED)
		{
			SyncLog::LOG(EROR, "ConnectSocket CreateTcpClient ,errno:ECONNABORTED m_remote_ip:%s", m_remote_ip.c_str());
		}
		else 
		{
			SyncLog::LOG(EROR, "ConnectSocket CreateTcpClient ,errno:%d m_remote_ip:%s", errno,  m_remote_ip.c_str());
		}
		return -1;
	}

	SyncLog::LOG(INFO, "TCP Client Connect Success Ret:%d",ret);

	return  ret;

}

int ConnectSocket::HandleClose()
{
	return m_socket.Close();
}

void ConnectSocket::CloseTcpClient()
{
	m_net_manager->GetReactor()->RemoveHandler(this);
	//m_socket.Close();
}

}

}

