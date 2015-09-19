#ifndef _NET_PACKET_HEAD_
#define _NET_PACKET_HEAD_

namespace Common
{

namespace NetReactor
{

#define NET_PACKET_HEAD_SIZE sizeof(NetPacketHead)

typedef struct
{
	uint32_t length;
	uint32_t sid;
	uint32_t uid;
	uint32_t cmd;
} __attribute__((packed)) NetPacketHead;


struct InnerPacketHead
{
	uint32_t length;
	uint32_t sid;
	uint32_t uid;
	uint32_t cmd;
	uint32_t hid;	// StreamSocket 描述符ID
};

}

}


#endif
