#ifndef _NET_REACTOR_
#define _NET_REACTOR_

#include "net_manager.h"

namespace Common
{

namespace NetReactor
{

	class Reactor
	{
	public:
		Reactor();

		virtual int RunReactorEventLoop() = 0;

		virtual int EndReactorEventLoop() = 0;

		virtual int  RegisterHandler(EventHandler *) = 0;

		virtual int RemoveHandler(EventHandler *) = 0;
	private:
		//SocketServer			m_scoket;
		NetManager *			m_net_manager;
	};
	
}

}

#endif

