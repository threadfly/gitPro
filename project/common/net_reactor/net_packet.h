#ifndef _NET_PACKET
#define _NET_PACKET

#include "net_share.h"

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
	
	public:
		int  m_buff_len;
		//TODO ��������ǲ��ǲ���... Ӧ������һ����̬�����İ�...
		char m_buff[NET_PACKET_BUFF_SIZE];
		
		long m_handler_id;
	};

}

}

#endif

