#include "net_send_pipe.h"

namespace Common
{

namespace NetReactor
{

SendPipe::SendPipe(NetManager * nm):
EventHandler(nm),
{
	//m_pipe.Init();	//构造函数中做这些操作肯定是有问题的
}

int SendPipe::GetFd()
{
}

int SendPipe::HandleInput()
{
}

int SendPipe::HandleOutput()
{
}

int SendPipe::HandleClose()
{
	m_pipe.CloseRead();
	m_pipe.CloseWrite();
	return 0;
}

int SendPipe::Init(int flags=0)
{
	return m_pipe.Init(flags);
}

}

}

