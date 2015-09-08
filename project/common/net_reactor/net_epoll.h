#ifdef _NET_EPOLL_
#define _NET_EPOLL_

#include <sys/epoll.h>

#include "net_reactor.h"
#include "log/sync_log.h"

namespace Common
{

namespace NetReactor
{
	#define MAX_EVENTS 100000
	class Epoll:public Reactor
	{
	public:
		Epoll();

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
