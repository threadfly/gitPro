#ifndef _SERVER_
#define _SERVER_

#include <sys/types.h>
#include <signal.h>

#include <iostream>
#include <map>
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

class BlockServer: public Thread
{
public:
	typedef std::map<int, int> FD_MAP;
	typedef std::map<int, Event *> EVENT_MAP;
public:
	BlockServer(string& ip, int port);
	~BlockServer(){};

	virtual  void Run();

	Event * AddEvent(int fd);
private:
	SocketServer	m_server;
	EVENT_MAP		m_event_map;
};

#endif
