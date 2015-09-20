#ifndef _NET_SHARE_
#define _NET_SHARE_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>

#include "log/sync_log.h"

/*
typedef struct sockaddr SA;
typedef struct socklen_t SL;
typedef struct sockaddr_in SAI;
*/
#define SA struct sockaddr
#define SL socklen_t
#define SAI struct sockaddr_in

#define ListenQ 1024
#define NET_PACKET_BUFF_SIZE 1024*4

#define EPOLL_REACTOR
#endif

