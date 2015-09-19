#ifndef _NET_EVENT_HANDLER_
#define _NET_EVENT_HANDLER_

#include "net_manager.h"

namespace Common
{

namespace NetReactor
{

	class EventHandler
	{
	public:
		EventHandler(NetManager *);

		virtual ~EventHandler(){};
		//virtual void DealEvent() = 0;
		virtual int GetFd() = 0;

		//virtual int Read() = 0;
		virtual int HandleInput() = 0;

		//virtual int Write() = 0;
		virtual int HandleOutput(const void *, size_t nbytes) = 0;

		virtual int HandleClose() = 0;

		/*
		virtual int HandleException() = 0;

		virtual int HandleTimeOut() = 0;
		*/
		/*Reactor * GetReactor();

		void	SetReactor(Reactor *);*/

		long GetId(){ return m_id;}
	protected:
		//Reactor	*		m_reactor;
		NetManager *	m_net_manager;

		long			m_id;
	};
}

}

#endif
