#ifndef _CLIENT_CONFIG_
#define _CLIENT_CONFIG_

#include <stdint.h>
#include <string>

using namespace std;

class LoadServerConfig
{
public:
	int LoadConfig(const char * xmlfile);
public:
	string		m_server_ip;
	uint32_t	m_server_port;
};

#endif
