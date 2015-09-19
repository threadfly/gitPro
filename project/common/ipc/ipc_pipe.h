#ifndef _COMMON_PIPE_
#define _COMMON_PIPE_

#include "ipc_share.h"

namespace Common
{

namespace IPC
{

	class Pipe
	{
	public:
		inline void CloseRead(){ ::close(m_pipefd[0]);}

		inline void CloseWrite(){ ::close(m_pipefd[1]);}

		int Read(void * buff, size_t nbytes);

		int Write(const void * buff, size_t nbytes);

		int Init(int flags = 0);
	public:
		int		m_pipefd[2];
	};

}

}

#endif
