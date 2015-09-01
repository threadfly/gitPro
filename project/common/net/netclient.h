#ifndef _NET_CLIENT_
#define _NET_CLIENT_
#include "net.h"

namespace Common
{

namespace Net
{
	class SocketClient
	{
	public:
		SocketClient(string& ip, int port, int bufsize = BUFSIZE);
		~SocketClient(){};
		int Connect();

	public:
		int Read(char * buff, int bufsize, int flag=0);

		int Write(char * buff, int bufsize, int flag=0);
	
		int Close();
	private:
		string				 m_ip;
		int					 m_port;
		int					 m_socketid;
		struct sockaddr_in m_servaddr;
	};
}

}



#endif
