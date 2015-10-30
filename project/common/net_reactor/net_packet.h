#ifndef _NET_PACKET
#define _NET_PACKET

#include "net_share.h"
#include "net_packet_head.h"

namespace Common
{

namespace NetReactor
{
	class EventHandler;

	class NetPacket
	{
	public:
		friend class NetManager;
	public:
		NetPacket(long);
		~NetPacket(){};

		int Write(const void * src, int len);

		int Read(void * buff, int len);
		
		int Clear();
	public:
		int  m_buff_len;
		//TODO 这个缓存是不是不够... 应该做成一个动态增长的吧...
		char m_buff[NET_PACKET_BUFF_SIZE];
		
		uint32_t m_handler_id;

		NetPacketHead m_head;
	};

}

}

#endif

