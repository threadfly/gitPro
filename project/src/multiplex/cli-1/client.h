#ifndef _CLIENT_
#define _CLIENT_
#include "thread.h"
#include "netclient.h"
#include "string.h"

using namespace Common::Net;
using namespace Common::Thread;

#define BUF_MAX_SIZE 1024

class BlockClient: public Thread
{
public:
	BlockClient(int id, string& ip, int port);
	~BlockClient(){};
	
	virtual void Run();

private:
	int				m_id;
	int				m_index;
	SocketClient	m_client;
};

BlockClient::BlockClient(int id,string& ip, int port):
Thread(false),
m_id(id),
m_index(0),
m_client(ip, port)
{
}

void BlockClient::Run()
{
	if (m_client.Connect() < 0)
	{
		std::cout << "clientid:"<< m_id<< " Connect Error" << std::endl;
		return;
	}

	char  buf[BUF_MAX_SIZE];
	snprintf(buf, BUF_MAX_SIZE, "I am client %d, happy see you! The num m_index is %d", m_id, m_index++);
	if (m_client.Write(buf, strlen(buf)) < 0)
	{
		std::cout << "clientid:"<< m_id<< " Write Error" << std::endl;
	}
	bzero(buf, BUF_MAX_SIZE);
	while(true)
	{
		int ret = m_client.Read(buf, BUF_MAX_SIZE);
		if ( ret  < 0)
		{
			std::cout << "clientid:"<< m_id<< " Read Error" << std::endl;
			perror("client Read");
		}
		else if (ret == 0)
		{
			std::cout << "clientid:"<< m_id<< " server stop me!!" << buf <<  std::endl;
			m_client.Close();
			break;
		}
		else 
		{
			std::cout << "clientid:"<< m_id<< " receive server:" << buf <<  std::endl;
		}
		bzero(buf, BUF_MAX_SIZE);
		SyncLog::LOG(INFO, "buf:%s", buf);
		snprintf(buf, BUF_MAX_SIZE, "I am client %d, happy see you! The num m_index is %d", m_id, m_index++);
		std::cout<<"!!!!!! :"<< buf <<std::endl;
		if (m_client.Write(buf, strlen(buf)) < 0)
		{
			std::cout << "clientid:"<< m_id<< " Write Error" << std::endl;
		}
		bzero(buf, BUF_MAX_SIZE);
	}
}

#endif
