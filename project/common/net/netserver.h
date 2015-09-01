#ifndef _NET_SERVER_
#define _NET_SERVER_
#include "net.h"

namespace Common
{
namespace Net
{
		class SocketServer
		{
		public:
			SocketServer(string& ip, int port, int bufsize = BUFSIZE);
			~SocketServer(){};
			/*
			 * @return 出错返回-1, 成功返回非负描述符
			 */
			int Accept(struct sockaddr *cliaddr, socklen_t * addrlen);
			
			int Work(struct sockaddr *cliaddr, socklen_t * addrlen);
			
			/*
			 * @param true: 设置为阻塞模式 false:设置为非阻塞模式
			 */
			int SetBlock(bool);
		private:
			int bind();

			int listen(int listenq = ListenQ);
	
		private:
			string					m_ip;
			int						m_port;
			int						m_socketid;
			struct sockaddr_in		m_servaddr;
		};
		
}

}



#endif
