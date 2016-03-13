/*
 * Inherit Reactor for Implement Selector Reactor
 */

//TODO

#ifndef _NET_SELECT_
#define _NET_SELECT_

namespace Common
{


namespace NetReactor
{
	using namespace Common::Log;
	class Reactor;
	class NetManager;
	class EventHandler;

	class Selector:public Reactor
	{
	public:
		Selector(NetManager *);
		
		virtual ~Selector() {};
	}

	public:
		virtual int RunReactorEventLoop();

		virtual int EndReactorEventLoop();

		virtual int  RegisterHandler(EventHandler *);

		virtual int RemoveHandler(EventHandler *);
	
	private:
		// ......
}



}

#endif
