#ifndef _NET_PACKET
#define _NET_PACKET

namespace Common
{

namespace Net
{

NetPacket::NetPacket():
m_buff_len(0)
{
}

int NetPacket::Write(const void * src, int len)
{
	len = len > PACKET_BUFF_SIZE ? PACKET_BUFF_SIZE : len;
	::memcpy((void *)m_buff, src, len);
	return len;
}

int NetPacket::Read(char * buff, int len)
{
	len = len > PACKET_BUFF_SIZE ? PACKET_BUFF_SIZE : len;
	::memcpy(buff, m_buff, len);
	return len;
}

}

}

#endif
