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
#include "sync_log.h"

using namespace std;

using namespace Common::Net;
using namespace Common::Thread;
using namespace Common::Time;
using namespace Common::Log;

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

class BlockServer: public Thread
{
public:
	BlockServer(string& ip, int port);
	virtual ~BlockServer(){};

	virtual  void Run();

	//Event * AddEvent(int fd);

private:
	AcceptServer	m_accept_server;
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

#endif

