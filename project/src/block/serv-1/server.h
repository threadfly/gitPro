#ifndef _SERVER_
#define _SERVER_

#include <sys/types.h>
#include <signal.h>

#include <iostream>
//#include <map>
#include <vector>
#include <queue>
#include <string.h>

#include "thread.h"
#include "netserver.h"
#include "sleeper.h"
#include "netevent.h"

using namespace Common::Net;
using namespace Common::Thread;
using namespace Common::Time;
using namespace std;

#define BUF_MAX_SIZE 1024

typedef std::vector< Event *> EVENT_VEC;
typedef std::queue<Event *> EVENT_QUEUE;

class AcceptServer: public Thread
{
public:
	AcceptServer(string& ip, int port);
	virtual ~AcceptServer(){};
	
	virtual void Run();
	
	void SetStop(bool);

	bool GetStop();

	void GetEvents(EVENT_VEC & vec);
private:
	SocketServer	m_server;
	EVENT_QUEUE		m_eventq;
	int				m_stop;
};

AcceptServer::AcceptServer(string & ip, int port):
Thread(true), // set detach state
m_server(ip, port)
{
}

void AcceptServer::SetStop(bool stop)
{
	// lock
	m_stop = stop;
	// unlock
}

bool AcceptServer::GetStop()
{
	return m_stop;
}

void AcceptServer::GetEvents(EVENT_VEC & vec )
{
	// lock 
	while (!m_eventq.empty()) 
	{
		vec.push_back(m_eventq.front());
		m_eventq.pop();
	}
	// unlock
}

void AcceptServer::Run()
{
	int connfd = 0;
	Event * pevent = NULL;
	m_stop = false;
	connfd = m_server.Work(NULL, NULL);
	if (connfd > 0 )
	{
		// lock
		pevent = new Event(connfd);
		if ( NULL != pevent)
		{
			m_eventq.push(pevent);
			pevent->SetFdLinger(1, 1);
			pevent->SetFdTimeOut();
		}
		// unlock
	}
	while(!m_stop)
	{
		connfd = m_server.Accept(NULL, NULL);
		if (connfd > 0 )
		{
			// lock
			pevent = new Event(connfd);
			if ( NULL != pevent)
			{
				m_eventq.push(pevent);
				pevent->SetFdLinger(1, 1);
				pevent->SetFdTimeOut();
			}
			// unlock
		}
	}

	SetStop(true);
}

class BlockServer: public Thread
{
public:
	BlockServer(string& ip, int port);
	virtual ~BlockServer(){};

	virtual  void Run();

	//Event * AddEvent(int fd);

private:
	AcceptServer	m_server;
	EVENT_VEC		m_event_vec;
};
/*
Event * BlockServer::AddEvent(int fd)
{
	if (m_event_map.find(fd) != m_event_map.end())
	{
		fprintf(stderr, "AddEvent have same fd:%d", fd);
		return NULL;
	}
	Event * pevent =  new Event(fd);
	m_event_map[fd] = pevent;
	return pevent;
}*/

BlockServer::BlockServer(string& ip, int port):
Thread(false),
m_server(ip, port)
{
	
}

void BlockServer::Run()
{
	m_server.Start();
	Sleeper slp(0,200000);
	char  buf[BUF_MAX_SIZE];
	bzero(buf, BUF_MAX_SIZE);
	EVENT_VEC vec;

	signal(SIGPIPE, SIG_IGN);
	while(true)
	{
		m_server.GetEvents(vec);
		if (!vec.empty())
		{
			m_event_vec.insert(m_event_vec.end(), vec.begin(), vec.end());
			vec.clear();
		}else if (m_event_vec.empty())
		{
			slp.Start();
			std::cout<<"Events is empty!! Waiting ... "<< std::endl;
			if (m_server.GetStop())
			{
				std::cout<<"Accept Thread is Stop!!"<< std::endl;
				break;
			}
			continue;
		}

		for(EVENT_VEC::iterator it = m_event_vec.begin();
				it != m_event_vec.end(); )
		{
			Event * pevt = (*it);
			int ret = 0;
			if ((ret = pevt->Read()) == 0)
			{	
				pevt->Close();
				std::cout << " Server close fd:" << pevt->GetFd()<< std::endl;
				delete pevt;
				//m_event_map.erase(it++);
				it = m_event_vec.erase(it);
				continue;
			} 
			else if (ret > 0)
			{
				int write_len = 0;
				int len = 0;
				if (( len = pevt->ReadBuf(buf, BUF_MAX_SIZE)) > 0)
				{
					std::cout<< "Server recv from fd:" << pevt->GetFd()<< " data:"<<buf << std::endl;
					//bzero(buf, BUF_MAX_SIZE);
					write_len = snprintf(buf+len, BUF_MAX_SIZE, "Server Welcome you, your fd is %d !!\n", pevt->GetFd());
					int ret = pevt->Write(buf, strlen(buf));
					std::cout << "Ret:"<< ret <<" write_len:" << write_len<<"  Write data:" << buf << std::endl;
				}
				++it;
				bzero(buf, BUF_MAX_SIZE);
			}
			else if (ret < 0)
			{
				int err = pevt->GetErro();
				if (err == EWOULDBLOCK)
				{
					fprintf(stderr, "socket fd:%d read timeout", pevt->GetFd());
					++it;
					continue;
				}
				else
				{
					pevt->Close();
					delete pevt;
					//m_event_map.erase(it++);
					it = m_event_vec.erase(it);
				} // else 
			} // else if
		} // for
	} // while 
	
	std::cout<<"BlockServer Thread Stop..."<< std::endl;
	//m_server.join();
}

#endif
