/*
 *  
 */

#include <sys/select.h>
#include "log/sync_log.h"
#include "net_select.h"
#include "net_share.h"
#include "net_event_handler.h"

namespace Common
{

namespace NetReactor
{
	Selector::Selector(NetManger * nm)
	:Reactor(nm)
	{
		//TODO
	}

	int Selector::RunReactorEventLoop()
	{
		//TODO
		return 1;
	}

	int Selector::EndReactorEventLoop()
	{
		// TODO 
		return 0;
	}

	int Selector::RegisterHandler(EventHandler * handler)
	{
		// TODO 
		return 1;
	}

	int Selector::RemoveHandler(EventHandler * handler)
	{
		// TODO 
		return 1;
	}
}

}

