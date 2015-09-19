#include "ipc_pipe.h"
#include "log/sync_log.h"

namespace Common
{

namespace IPC
{

using namespace Common::Log;

int Pipe::Read(void * buff, size_t nbytes)
{
	if ( NULL == buff )
	{
		SyncLog::LOG(EROR, "Pipe Read param buff is NULL");
		return -1;
	}

	int num = ::read(m_pipefd[0], buff, nbytes);

	if ( -1 == num )
	{
		SyncLog::LOG(EROR, "Pipe Read read , errno:%d", errno);
		return -1;
	}
	return num;
}

int Pipe::Write(const void * buff, size_t nbytes)
{
	if ( NULL == buff || nbytes == 0)
	{
		SyncLog::LOG(EROR, "Pipe Write param buff is NULL Or nbytes == 0");
		return -1;
	}

	int num = ::write(m_pipefd[1], buff, nbytes);

	if ( -1 == num )
	{
		SyncLog::LOG(EROR, "Pipe Write write, errno:%d", errno);
		return -1;
	}
	return num;
}

int Pipe::Init(int flags)
{
	int ret = ::pipe2(m_pipefd, flags);
	if (-1 == ret )
	{
		if ( errno == EFAULT)
		{
			SyncLog::LOG(EROR, "Pipe Init pipe2 errno:EFAULT");
		}
		else if ( errno == EINVAL )
		{
			SyncLog::LOG(EROR, "Pipe Init pipe2 errno::EINVAL");
		}
		else
		{
			SyncLog::LOG(EROR, "Pipe Init pipe2 errno::%d", errno);
		}
	}

	return ret;
}

}

}

