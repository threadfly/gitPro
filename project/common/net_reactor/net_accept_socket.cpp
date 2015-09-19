#include "net_accept_socket.h"
#include "net_manager.h"
#include "log/sync_log.h"
#include "net_share.h"
#include "net_stream_socket.h"
#include "net_reactor.h"

namespace Common
{

namespace NetReactor
{

AcceptSocket::AcceptSocket(NetManager * nm):
EventHandler(nm)
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
	SL sl;
	::memset(&sai, 0 ,sizeof(sai));
	
	fd = m_socket.Accept((SA *)&sai, &sl);
	if ( -1 == fd )
		return -1;
	
	char remote_ip[32] = {0};
	const char * ret0 = ::inet_ntop(AF_INET, (const void *)&sai.sin_addr, remote_ip, sizeof(remote_ip));
	if ( NULL == ret0 )
	{
		SyncLog::LOG(EROR, "AcceptSocket HandleInput inet_ntop Error, errno:%d", errno);
		return -1;
	}
	int remote_port = (int)::ntohs(sai.sin_port);
	string ip(remote_ip);
	StreamSocket * psskt =  new StreamSocket(m_net_manager, fd, m_local_ip, m_local_port, ip, remote_port );

	int ret = m_net_manager->GetReactor()->RegisterHandler(psskt);
	if ( 0 !=  ret)
	{
		SyncLog::LOG(EROR, "AcceptSocket HandleInput RegisterEventHandler Error");
		return -1;
	}
	
	SyncLog::LOG(INFO, "AcceptSocket HandleInput RegisterEventHandler New Client Come in, ip:%s port:%d", ip.c_str(), remote_port);
	return 0;
}

int AcceptSocket::HandleOutput(const void * buff, size_t nbytes)
{
	return 0;
}

int AcceptSocket::CreateTcpServer(const string & ip, const int port )
{
	int ret = -1;
	ret = m_socket.Init(AF_INET, SOCK_STREAM, 0);
	if ( -1 == ret )
	{
		SyncLog::LOG(EROR, "AcceptSocket CreateTcpServer Error, ret:%d", ret);
		return ret;
	}
	
	int open = 1;
	ret = m_socket.SetSocketOpt(SOL_SOCKET, 
						 SO_REUSEADDR, 
						 &open, 
						 sizeof(open));
	if ( ret != 0)
	{
		SyncLog::LOG(EROR, "AcceptSocket CreateTcpServer SetSocketOpt Error, ret:%d", ret);
		return ret;
	}

	std::cout << ip << ":" << port << std::endl;
	std::cout << __FILE__ << ":" << __LINE__ << std::endl;
	SyncLog::LOG(INFO, " %s:%d AcceptSocket CreateTcpServer ", __FILE__, __LINE__);
	m_local_ip = ip;
	m_local_port = port;

	SAI sai;
	::memset(&sai, 0, sizeof(sai));
	sai.sin_family = AF_INET;
	if ( m_local_ip != "")
	{
		SyncLog::LOG(DEBUG, "AcceptSocket CreateTcpServer , m_local_ip:%s", m_local_ip.c_str());
		if ( 0 >=  inet_pton(AF_INET, m_local_ip.c_str(), &sai.sin_addr))
		{
			SyncLog::LOG(EROR, "AcceptSocket CreateTcpServer inet_pton error, m_local_ip:%s", m_local_ip.c_str());
			return -1;
		}
	}
	else
	{
		SyncLog::LOG(DEBUG, "AcceptSocket CreateTcpServer , m_local_ip:INADDR_ANY");
		sai.sin_addr.s_addr = htonl(m_local_port);
	}
	sai.sin_port = htons(m_local_port);
	ret = m_socket.Bind((SA *)&sai, sizeof(sai) );
	if ( -1 == ret )
	{
		SyncLog::LOG(EROR, "AcceptSocket CreateTcpServer Bind error, m_local_ip:%s m_local_port:%d", m_local_ip.c_str(), m_local_port);
		return -1;
	}

	ret = m_socket.Listen();
	if ( -1 == ret )
	{
		SyncLog::LOG(EROR, "AcceptSocket CreateTcpServer Listen error, m_local_ip:%s m_local_port:%d", m_local_ip.c_str(), m_local_port);
		return -1;
	}
	
	return ret;
}

int AcceptSocket::HandleClose()
{
	return m_socket.Close();
}

void AcceptSocket::CloseTcpServer()
{
	m_net_manager->GetReactor()->RemoveHandler(this);
	//m_socket.Close();
}

}

}

