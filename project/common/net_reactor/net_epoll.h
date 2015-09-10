#ifndef _NET_EPOLL_
#define _NET_EPOLL_

#include "net_reactor.h"

namespace Common
{

namespace NetReactor
{
	using namespace Common::Log;
	class Reactor;
	class NetManager;
	class EventHandler;

	#define MAX_EVENTS 100000
	class Epoll:public Reactor
	{
	public:
		Epoll(NetManager *);

		virtual ~Epoll(){};

	public:
		virtual int RunReactorEventLoop();

		virtual int EndReactorEventLoop();

		virtual int  RegisterHandler(EventHandler *);

		virtual int RemoveHandler(EventHandler *);

	private:
		int						m_epfd;
		int						m_events_len;
		struct epoll_event *	m_events;
	};

}

}

#endif

