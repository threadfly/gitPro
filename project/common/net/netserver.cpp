#include "netserver.h"
#include "fcntl.h"

namespace Common
{

namespace Net
{

SocketServer::SocketServer(string& ip, int port, int bufsize)
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

int SocketServer::bind()
{
	bzero(&m_servaddr, sizeof(m_servaddr));
	m_servaddr.sin_family = AF_INET;
	std::cout << "bind m_ip:"<< m_ip << std::endl;
	if (m_ip != "")
	{
		std::cout << "bind 1" << std::endl;
		if (0 >= inet_pton(AF_INET, m_ip.c_str(), &m_servaddr.sin_addr))
		{
			std::cout<< "inet_aton error " << "ip:" << m_ip << std::endl;
			return -1;
		}
	}
	else 
	{
		std::cout << "bind 2" << std::endl;
		m_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	m_servaddr.sin_port = htons(m_port);

	if ( ::bind(m_socketid, (SA *)&m_servaddr, sizeof(m_servaddr)) < -1)
	{
		std::cout <<"SocketServer Bind Error" << std::endl;
		return -1;
	}
	return 0;
}

int SocketServer::listen(int listenq)
{
	char *ptr;
	if ((ptr = getenv("LISTENQ")) != NULL)
		listenq = atoi(ptr);

	if (::listen(m_socketid, listenq) < 0)
	{
		std::cout<<"listen error"<<std::endl;
		return -1;
	}
	
	return 0;
}

int SocketServer::Accept(struct sockaddr *cliaddr, socklen_t * addrlen)
{
	/*if (NULL == cliaddr || NULL == addrlen)
	{
		std::cout<<"accept param error"<<std::endl;
		return -1;
	}*/
	int connfd = accept(m_socketid, cliaddr, addrlen);
	if ( connfd < 0)
	{
		if (errno == EAGAIN)
		{
			std::cout << "Accept Error:EAGAIN"<< std::endl;
		}
		else if (errno == EWOULDBLOCK)
		{
			std::cout << "Accept Error:EAGAIN"<< std::endl;
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
		perror(NULL);
		return -1;
	}
	return connfd;
}

int SocketServer::Work(struct sockaddr *cliaddr, socklen_t * addrlen)
{
	bind();

	int ret = listen();
	if (ret != 0)
		return ret;

	ret = Accept(cliaddr, addrlen);

	return ret;
}

int SocketServer::SetBlock(bool flag)
{
	int flags;
	if ((flags = fcntl(m_socketid, F_GETFL, 0)) < 0 )
	{
		std::cout << "F_GETFL Error" << std::endl;
		return -1;
	}
	if ( !flag )
	{
		flags |= O_NONBLOCK;
		if (fcntl(m_socketid, F_SETFL, flags) < 0)
		{
			std::cout << "F_SETFL Error" << std::endl;
			return -1;
		}
	} else {
		flags &= ~O_NONBLOCK;
		if (fcntl(m_socketid, F_SETFL, flags) < 0)
		{
			std::cout << "F_SETFL Error" << std::endl;
			return -1;
		}
	}
	return 0;
}

}

}


