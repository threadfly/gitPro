#include "server.h"

Event * BlockServer::AddEvent(int fd)
{
	if (m_event_map.find(fd) != m_event_map.end())
	{
		fprintf(stderr, "AddEvent have same fd:%d", fd);
		return NULL;
	}
	Event * pevent =  new Event(fd);
	m_event_map[fd] = pevent;
	return pevent;
}

BlockServer::BlockServer(string& ip, int port):
Thread(false),
m_server(ip, port)
{
	
}

void BlockServer::Run()
{
	Sleeper slp(0,200000);
	char  buf[BUF_MAX_SIZE];
	bzero(buf, BUF_MAX_SIZE);
	if ( m_server.SetBlock(false) < 0)
	{
		std::cout << "m_server.SetBlock error" << std::endl;
		return;
	}
	int connfd = m_server.Work(NULL, NULL);
	if (connfd < 0)
	{
		std::cout << "Work error"<< std::endl;
		//return;
	}else 
	{
		//fdMap[connfd] = connfd;
		Event * p =	AddEvent(connfd);
		if (NULL == p)
		{
			perror("AddEvent return NULL");
			return;
		}
		p->SetFdLinger(1, 5);
		p->SetFdTimeOut();
	}
	signal(SIGPIPE, SIG_IGN);
	while(true)
	{
		for(EVENT_MAP::iterator it = m_event_map.begin();
				it != m_event_map.end(); )
		{
			int ret = 0;
			if ((ret = (it->second)->Read()) == 0)
			{	
				(it->second)->Close();
				std::cout << " Server close fd:" << it->first << std::endl;
				delete it->second;
				m_event_map.erase(it++);
				continue;
			} 
			else if (ret > 0)
			{
				int write_len = 0;
				int len = 0;
				if (( len = (it->second)->ReadBuf(buf, BUF_MAX_SIZE)) > 0)
				{
					std::cout<< "Server recv from fd:" << it->first << " data:"<<buf << std::endl;
					//bzero(buf, BUF_MAX_SIZE);
					write_len = snprintf(buf+len, BUF_MAX_SIZE, "Server Welcome you, your fd is %d !!\n", it->first);
					int ret = (it->second)->Write(buf, strlen(buf));
					std::cout << "Ret:"<< ret <<" write_len:" << write_len<<"  Write data:" << buf << std::endl;
				}
				++it;
				bzero(buf, BUF_MAX_SIZE);
			}
			else if (ret < 0)
			{
				int err = (it->second)->GetErro();
				if (err == EWOULDBLOCK)
				{
					fprintf(stderr, "socket fd:%d read timeout", it->first);
					++it;
					continue;
				}
				else
				{
					(it->second)->Close();
					delete it->second;
					m_event_map.erase(it++);
				}
			}
		}

		connfd = m_server.Accept(NULL, NULL);
		if (connfd > 0)
		{
			//fdMap[connfd] = connfd;
			//AddEvent(connfd);
			Event * p =	AddEvent(connfd);
			if (NULL == p)
			{
				perror("Below AddEvent return NULL");
				return;
			}
			p->SetFdLinger(1, 5);
			p->SetFdTimeOut();
		}else{
			//std::cout<< "Server sleeping ..." << std::endl;
			//slp.Start();
		}
	}
}

