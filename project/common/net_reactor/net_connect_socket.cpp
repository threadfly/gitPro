#include "net_connect_socket.h"
#include "net_manager.h"
#include "log/sync_log.h"
#include "net_share.h"
#include "net_reactor.h"

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
	return 0;
}

int ConnectSocket::HandleOutput()
{
	SyncLog::LOG(INFO, "Client ConnectSocket HandleOutput");
	return 0;
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
		//TODO 客户端可以不指定远程IP么?
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

