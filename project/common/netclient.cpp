#include "netclient.h"

namespace Common
{

namespace Net
{

SocketClient::SocketClient(string& ip, int port, int bufsize)
{
	m_socketid = socket(AF_INET, SOCK_STREAM, 0);
	int flag = 1;

	int ret = setsockopt(m_socketid,
						 SOL_SOCKET, 
						 SO_RCVBUF, 
						 &bufsize, 
						 sizeof(bufsize));
	if ( ret != 0)
		std::cout<<"setscokopt RCVBUF error"<<std::endl;

	ret = setsockopt(m_socketid, 
						 SOL_SOCKET, 
						 SO_SNDBUF, 
						 &bufsize, 
						 sizeof(bufsize));
	if ( ret != 0)
		std::cout<<"setscokopt SNDBUF error"<<std::endl;

	ret = setsockopt(m_socketid, 
						 SOL_SOCKET, 
						 SO_REUSEADDR, 
						 &flag, 
						 sizeof(bufsize));
	if ( ret != 0)
		std::cout<<"setscokopt RUSBUF error"<<std::endl;

	ret = setsockopt(m_socketid,
						 SOL_SOCKET, 
						 SO_KEEPALIVE,
						 &flag, 
						 sizeof(bufsize));
	if ( ret != 0)
		std::cout<<"setscokopt KEEPALIVE error"<<std::endl;

	m_ip = ip;
	m_port = port;
}

int  SocketClient::Connect()
{
	bzero(&m_servaddr, sizeof(m_servaddr));

	m_servaddr.sin_family = AF_INET;
	if (m_ip != "")
	{
		if (0 == inet_pton(AF_INET, m_ip.c_str(), &m_servaddr.sin_addr))
		{
			std::cout<< "inet_aton error " << "ip:" << m_ip << std::endl;
			return -1;
		}
	}
	else 
	{
		m_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	m_servaddr.sin_port = htons(m_port);

	int ret = ::connect(m_socketid, (SA *)&m_servaddr, sizeof(m_servaddr));
	if ( ret < 0)
	{
		if (errno == EAGAIN)
		{
			std::cout << "Accept Error:EAGAIN"<< std::endl;
		}
		else if (errno == EWOULDBLOCK)
		{
			std::cout << "Accept Error:EWOULDBLOCK"<< std::endl;
		}
		else if (errno == EFAULT)
		{
			std::cout << "Accept Error:EFAULT"<< std::endl;
		}
		else if (errno == ECONNABORTED)
		{
			std::cout << "Accept Error:ECONNABORTED"<< std::endl;
		}
		else 
		{
			std::cout << "Accept Error, errno"<< errno<<std::endl;
		}
		perror("SocketClient Connet!!");
		return -1;
	}
	
	return 0;
}

int SocketClient::Read(char * buff, int bufsize, int flag)
{
	if (bufsize < 1 || NULL == buff)
	{
		std::cout << "Read Error " << "buff:" << buff << "  bufsize:"<< bufsize << std::endl;
		return -1;
	}
	
	return recv(m_socketid, (void *)buff, (size_t)bufsize, flag);

}

int SocketClient::Write(char * buff, int bufsize, int flag)
{
	if (bufsize < 1 || NULL == buff)
	{
		std::cout << "Write Error " << "buff:" << buff << "  bufsize:"<< bufsize<< std::endl;
		return -1;
	}

	return send(m_socketid, (void*)buff, (size_t)bufsize, flag);
}

int SocketClient::Close()
{
	return ::close(m_socketid);
}

}

}
