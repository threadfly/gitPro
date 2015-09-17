#ifndef _COMMON_PIPE_
#define _COMMON_PIPE_

namespace Common
{

namespace IPC
{

	class Pipe
	{
	public:
		void CloseRead(){ ::close(m_pipefd[0]);}

		void CloseWrite(){ ::close(m_pipefd[1]);}

		int Read(void * buff, size_t nbytes);

		int Write(const void * buff, size_t nbytes);

		int Init(int flags = 0);
	private:
		int		m_pipefd[2];
	};

}

}

#endif
