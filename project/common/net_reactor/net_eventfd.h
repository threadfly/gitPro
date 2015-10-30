#ifndef _NET_EVENTFD_
#define _NET_EVENTFD_

#include <sys/eventfd.h>

#include <net_event_handler.h>

namespace Common
{

namespace NetReactor
{
	class NetManager;

	class LinuxEventFd:public EventHandler
	{
	public:
		LinuxEventFd(NetManager *);

		virtual ~LinuxEventFd(){};

		virtual int GetFd();

		virtual int HandleInput();

		virtual int HandleOutput(const void *, size_t nbytes);
		
		virtual int HandleClose();

		//int Write(const void * buff, size_t nbytes);
		int Signal();

		int Init();
	private:
		int m_efd;
	};
}

}
#endif

