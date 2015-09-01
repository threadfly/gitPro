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
			 * @return ������-1, �ɹ����طǸ�������
			 */
			int Accept(struct sockaddr *cliaddr, socklen_t * addrlen);
			
			int Work(struct sockaddr *cliaddr, socklen_t * addrlen);
			
			/*
			 * @param true: ����Ϊ����ģʽ false:����Ϊ������ģʽ
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
