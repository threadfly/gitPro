#ifndef _NET_
#define _NET_

#include "sys/socket.h"
#include "arpa/inet.h"
#include "netinet/in.h"

#include "strings.h"
#include "stdlib.h"
#include "stdio.h"
#include "errno.h"
#include <iostream>
using std::string;
using std::cout;
using std::endl;

#define SA struct sockaddr
#define BUFSIZE 65535
#define ListenQ	1024

#endif

