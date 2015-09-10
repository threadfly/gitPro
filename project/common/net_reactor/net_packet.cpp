#include "net_packet.h"

namespace Common
{

namespace NetReactor
{

NetPacket::NetPacket(long id):
m_buff_len(0),
m_handler_id(id)
{
}

int NetPacket::Write(const void * src, int len)
{
	memset(m_buff, 0, NET_PACKET_BUFF_SIZE);
	len = len > NET_PACKET_BUFF_SIZE ? NET_PACKET_BUFF_SIZE : len;
	::memcpy((void *)m_buff, src, len);
	return len;
}

int NetPacket::Read(void * buff, int len)
{
	len = len > NET_PACKET_BUFF_SIZE ? NET_PACKET_BUFF_SIZE : len;
	::memcpy(buff, m_buff, len);
	return len;
}

}

}

