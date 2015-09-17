#include "net_packet.h"
#include "log/sync_log.h"
using namespace Common::Log;

namespace Common
{

namespace NetReactor
{

NetPacket::NetPacket(long id):
m_buff_len(0),
m_handler_id(id)
{
	::memset(m_buff, 0, NET_PACKET_BUFF_SIZE);
}

int NetPacket::Write(const void * src, int len)
{
	memset(m_buff, 0, NET_PACKET_BUFF_SIZE);
	len = len > NET_PACKET_BUFF_SIZE ? NET_PACKET_BUFF_SIZE : len;
	::memcpy((void *)m_buff, src, len);
	SyncLog::LOG(INFO, "NetPacket Write:%s", m_buff);
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

