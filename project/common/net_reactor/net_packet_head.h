#ifndef _NET_PACKET_HEAD_
#define _NET_PACKET_HEAD_

namespace Common
{

namespace NetReactor
{

typedef struct
{
	uint32_t length;
	uint32_t sid;
	uint32_t uid;
} __attribute__((packed)) NetPacketHead;

struct InnerPacketHead
{
	uint32_t length;
	uint32_t sid;
	uint32_t uid;
	uint32_t hid;	// StreamSocket 描述符ID
};

}

}


#define
