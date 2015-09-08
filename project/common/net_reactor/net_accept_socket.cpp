#include "net_accept_socket.h"

namespace Common
{

namespace NetReactor
{

AcceptSocket::AcceptSocket(NetManager * nm):
m_net_manager(nm)
{
}

int AcceptSocket::GetFd()
{
	return m_socket.GetSocketId();
}

int AcceptSocket::HandleInput()
{
	int fd;
	SAI sai;
	::memset(&sai, 0 ,sizeof(sai));
	
	fd = m_socket.Accept((SA *)&sai, sizeof(sai) );
	if ( -1 == fd )
		return -1;
	
	char remont_ip[32] = {0};
	char * ret0 = ::inet_ntop(AF_INET, &sai.sin_addr, remote_ip, sizeof(remote_ip));
	if ( NULL == ret0 )
	{
		SyncLog::Log(EROR, "AcceptSocket HandleInput inet_ntop Error, errno:%d", %d);
		return -1;
	}
	int remote_port = (int)::ntohs(sai.sin_port);
	string ip(remote_ip);
	StreamSocket * psskt =  new StreamSocket(m_net_manager, m_local_ip, m_local_port, ip, remote_port );

	int ret = m_net_manager->GetReactor()->RegisterEventHandler(psskt);
	if ( 0 !=  ret)
	{
		SyncLog::Log(EROR, "AcceptSocket HandleInput RegisterEventHandler Error");
		return -1;
	}
	
	SyncLog::Log(INFO, "AcceptSocket HandleInput RegisterEventHandler New Client Come in, ip:%s port:%d", ip, remote_port);
	return 0;
}

int AcceptSocket::HandleOutput()
{
	return 0;
}

int AcceptSocket::CreateTcpServer(const string & ip, const int port )
{
	int ret = -1;
	ret = m_socket.Init(AF_INET, SOCK_STREAM, 0);
	if ( -1 == ret )
	{
		SyncLog::Log(EROR, "AcceptSocket CreateTcpServer Error, ret:%d", ret);
		return ret;
	}
	
	m_local_ip = ip;
	m_local_port = port;

	SAI sai;
	::memset(&sai, 0, sizeof(sai));
	sai.sin_family = AF_INET;
	if ( m_local_ip != "")
	{
		SyncLog::Log(DEBUG, "AcceptSocket CreateTcpServer , m_local_ip:%s", m_local_ip);
		if ( 0 >=  inet_pton(AF_INET, m_ip.c_str(), &sai.sin_addr))
		{
			SyncLog::Log(EROR, "AcceptSocket CreateTcpServer inet_pton error, m_local_ip:%s", m_local_ip);
			return -1;
		}
	}
	else
	{
		SyncLog::Log(DEBUG, "AcceptSocket CreateTcpServer , m_local_ip:INADDR_ANY");
		sai.sin_addr.s_addr = htonl(m_local_port);
	}
	sai.sin_port = htons(m_local_port);
	ret = m_socket.Bind((SA *)&sai, sizeof(sai) );
	if ( -1 == ret )
	{
		SyncLog::Log(EROR, "AcceptSocket CreateTcpServer Bind error, m_local_ip:%s m_local_port:%d", m_local_ip, m_local_port);
		return -1;
	}

	ret = m_socket.Listen();
	if ( -1 == ret )
	{
		SyncLog::Log(EROR, "AcceptSocket CreateTcpServer Listen error, m_local_ip:%s m_local_port:%d", m_local_ip, m_local_port);
		return -1;
	}
	
	return ret;
}

int AcceptSocket::HandleClose()
{
	m_socket.Close();
}

void AcceptSocket::CloseTcpServer()
{
	m_reactor->RemoveHandler(this);
	//m_socket.Close();
}

}

}

