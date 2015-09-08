#ifndef _NET_EVENT_HANDLER_
#define _NET_EVENT_HANDLER_

#include "net_reactor.h"

namespace Common
{

namespace NetReactor
{
	class EventHandler
	{
	public:
		//virtual void DealEvent() = 0;
		virtual int GetFd() = 0;

		//virtual int Read() = 0;
		virtual int HandleInput() = 0;

		//virtual int Write() = 0;
		virtual int HandleOutput() = 0;

		virtual int HandleClose() = 0;

		/*virtual int HandleException() = 0;

		virtual int HandleTimeOut() = 0;
		*/
		/*Reactor * GetReactor();

		void	SetReactor(Reactor *);
	private:
		Reactor	*		m_reactor;*/
	};
}

}

#endif
