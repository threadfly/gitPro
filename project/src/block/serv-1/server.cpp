#include "server.h"

AcceptServer::AcceptServer(string & ip, int port):
Thread(true), // set detach state
m_server(ip, port)
{
}

void AcceptServer::SetStop(bool stop)
{
	m_stop = stop;
}

bool AcceptServer::GetStop()
{
	return m_stop;
}

void AcceptServer::GetEvents(EVENT_VEC & vec )
{
	// lock 
	GetMutex().Lock();
	while (!m_eventq.empty()) 
	{
		vec.push_back(m_eventq.front());
		m_eventq.pop();
	}
	// unlock
	GetMutex().UnLock();
}

void AcceptServer::Run()
{
	int connfd = 0;
	Event * pevent = NULL;
	m_stop = false;
	connfd = m_server.Work(NULL, NULL);
	if (connfd > 0 )
	{
		pevent = new Event(connfd);
		if ( NULL != pevent)
		{
			pevent->SetFdLinger(1, 1);
			pevent->SetFdTimeOut();

			// lock
			GetMutex().Lock();

			m_eventq.push(pevent);
			
			GetCond().Signal();
			// unlock
			GetMutex().UnLock();
		}
	}
	while(!m_stop)
	{
		connfd = m_server.Accept(NULL, NULL);
		if (connfd > 0 )
		{
			pevent = new Event(connfd);
			if ( NULL != pevent)
			{
				pevent->SetFdLinger(1, 1);
				pevent->SetFdTimeOut();

				// lock
				GetMutex().Lock();

				m_eventq.push(pevent);

				GetCond().Signal();
				// unlock
				GetMutex().UnLock();
			}
		}
	}

	SetStop(true);
}

BlockServer::BlockServer(string& ip, int port):
Thread(false),
m_accept_server(ip, port)
{
	
}

void BlockServer::Run()
{
	m_accept_server.Start();
	Sleeper slp(0,200000);
	char  buf[BUF_MAX_SIZE];
	bzero(buf, BUF_MAX_SIZE);
	EVENT_VEC vec;

	signal(SIGPIPE, SIG_IGN);
	while(true)
	{
		m_accept_server.GetEvents(vec);
		if (!vec.empty())
		{
			m_event_vec.insert(m_event_vec.end(), vec.begin(), vec.end());
			vec.clear();
		}else if (m_event_vec.empty())
		{
			//slp.Start();
			m_accept_server.GetMutex().Lock();
			SyncLog::LOG(INFO, "Events is empty!! Waiting...\n");
			m_accept_server.GetCond().Wait(m_accept_server.GetMutex());
			m_accept_server.GetMutex().UnLock();
			if (m_accept_server.GetStop())
			{
				std::cout<<"Accept Thread is Stop!!"<< std::endl;
				break;
			}
			//m_accept_server.GetEvents(vec);
			continue;
		}

		for(EVENT_VEC::iterator it = m_event_vec.begin();
				it != m_event_vec.end(); )
		{
			Event * pevt = (*it);
			int ret = 0;
			if ((ret = pevt->Read()) == 0)
			{	
				pevt->Close();
				std::cout << " Server close fd:" << pevt->GetFd()<< std::endl;
				delete pevt;
				//m_event_map.erase(it++);
				it = m_event_vec.erase(it);
				continue;
			} 
			else if (ret > 0)
			{
				int write_len = 0;
				int len = 0;
				if (( len = pevt->ReadBuf(buf, BUF_MAX_SIZE)) > 0)
				{
					std::cout<< "Server recv from fd:" << pevt->GetFd()<< " data:"<<buf << std::endl;
					//bzero(buf, BUF_MAX_SIZE);
					write_len = snprintf(buf+len, BUF_MAX_SIZE, "Server Welcome you, your fd is %d !!\n", pevt->GetFd());
					int ret = pevt->Write(buf, strlen(buf));
					std::cout << "Ret:"<< ret <<" write_len:" << write_len<<"  Write data:" << buf << std::endl;
				}
				++it;
				bzero(buf, BUF_MAX_SIZE);
			}
			else if (ret < 0)
			{
				int err = pevt->GetErro();
				if (err == EWOULDBLOCK)
				{
					fprintf(stderr, "socket fd:%d read timeout", pevt->GetFd());
					++it;
					continue;
				}
				else
				{
					pevt->Close();
					delete pevt;
					//m_event_map.erase(it++);
					it = m_event_vec.erase(it);
				} // else 
			} // else if
		} // for
	} // while 
	
	std::cout<<"BlockServer Thread Stop..."<< std::endl;
	//m_server.join();
}

