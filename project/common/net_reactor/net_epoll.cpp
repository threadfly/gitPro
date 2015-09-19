#include <sys/epoll.h>
#include "log/sync_log.h"
#include "net_epoll.h"
#include "net_share.h"
#include "net_event_handler.h"

namespace Common
{

namespace NetReactor
{

	Epoll::Epoll(NetManager * nm):
	Reactor(nm)
	{
		m_events = new struct epoll_event[MAX_EVENTS];

		m_epfd = ::epoll_create(100);

		if ( -1 == m_epfd)
		{
			SyncLog::LOG(EROR, "epoll_create error!!");
			exit(EXIT_FAILURE);
		}
		
	}

	int Epoll::RunReactorEventLoop()
	{
		int nfds = ::epoll_wait(m_epfd, m_events, MAX_EVENTS, -1); // 最长等待500毫秒
		for (int i = 0; i < nfds; ++i)
		{
			EventHandler * peh = (EventHandler *)(m_events[i].data.ptr);
			if ( NULL == peh )
			{
				SyncLog::LOG(EROR, "Epoll RunReactorEventLoop peh is NULL!!");
				continue;
			}

			if ( m_events[i].events & EPOLLIN )
			{
				int ret = peh->HandleInput();
				if (-2 == ret)
				{
					RemoveHandler(peh);
					peh->HandleClose();
					m_net_manager->RemoveHandlerId(peh->GetId());
					delete peh;
				}

			}

			if ( m_events[i].events & EPOLLOUT)
			{
				SyncLog::LOG(EROR, "Epoll RunReactorEventLoop epoll_wait EPOLLOUT");
				//peh->HandleOutput();
			}

			if ( m_events[i].events & EPOLLERR || m_events[i].events & EPOLLHUP)
			{
				// TODO peh->HandleException();
				//RemoveHandler(peh);
			}
		}
		return nfds;
	}

	int Epoll::EndReactorEventLoop()
	{
		// TODO 这里该怎么处理那些handler? 怎么delete?
		return 0;
	}

	int Epoll::RegisterHandler(EventHandler * handler)
	{
		struct epoll_event ev;

		::memset(&ev, 0 , sizeof(ev));
		ev.data.ptr = (void *)handler;
		ev.events = EPOLLIN ; // 触发方式采用默认的水平触发

		int ret = ::epoll_ctl(m_epfd, EPOLL_CTL_ADD, handler->GetFd(), &ev);
		if ( 0 != ret)
		{
			if ( errno == EEXIST)
			{
				SyncLog::LOG(EROR, "Epoll RegisterHandler Error, errno:EEXIST");
				return -1;
			} 
			else 
			{
				SyncLog::LOG(EROR, "Epoll RegisterHandler Error, errno:%d", errno);
				return -1;
			}

		}

		return 0;
	}

	int Epoll::RemoveHandler(EventHandler * handler)
	{
		struct epoll_event ev;
		// TODO Linux 内核版本为 2.6.9之前的内核版本需要这个ev
		::epoll_ctl(m_epfd, EPOLL_CTL_DEL, handler->GetFd(), &ev);
		
		SyncLog::LOG(INFO, "Epoll RemoveHandler Close Handler");
		//handler->HandleClose();
		
		//m_net_manager->RemoveHandlerId(handler->GetId());
		// TODO 这里处理对否?
		//delete handler;

		return 0;
	}
}

}
