#ifndef _NET_SEND_PIPE_
#define _NET_SEND_PIPE_

#include "net_event_handler.h"
#include "ipc/ipc_pipe.h"

namespace Common
{

namespace NetReactor
{

class SendPipe: public EventHandler
{
public:
	SendPipe(NetManager *);

	virtual ~SendPipe(){};

	virtual int GetFd();

	virtual int HandleInput();

	virtual int HandleOutput();

	virtual int HandleClose();
	
	int Init(int flags=0);
private:
	Pipe	m_pipe;
};

}

}


#endif
