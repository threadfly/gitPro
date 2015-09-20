#include <signal.h>
#include "net_manager.h"
#include "sleeper.h"
#include "net_packet.h"
#include "sync_log.h"
#include "cmd.pb.h"
#include "cs.pb.h"

using namespace Common::NetReactor;
using namespace Common::Thread;
using namespace Common::Time;
using namespace Common::Log;

//protocol
using namespace cs;

int main()
{
	//char response[1024] = {0};
	NetManager g_net_manager;
	NetPacket * packet = NULL;
	if ( -1 == g_net_manager.Work())
	{
		return -1;
	}
	Sleeper slp(0, 120000);
	signal(SIGPIPE, SIG_IGN);
	while(true)
	{
		packet = g_net_manager.GetRecvNetPacket();
		if ( NULL != packet )
		{
			switch (packet->m_head.cmd)
			{
				case ID_C2S_Login:
				{
					C2S_Login login;
					string message(packet->m_buff, packet->m_buff_len);
					login.ParseFromString(message);
					SyncLog::LOG(INFO, "Server Welecome id:%d name:%s", login.id(), login.name().c_str());
					
					S2C_Login s2c_login;
					s2c_login.set_ret(RET_SUCCESS);
					string ret;
					s2c_login.SerializeToString(&ret);
					
					InnerPacketHead ihead;
					ihead.length = ret.length();
					ihead.sid = packet->m_head.sid;
					ihead.uid = packet->m_head.uid;
					ihead.cmd = ID_S2C_Login;
					ihead.hid = packet->m_handler_id;

					g_net_manager.SendMessage(ihead, ret.c_str());
					delete packet;
					packet = NULL;
				}
					break;
				default:
					SyncLog::LOG(EROR, "main cmd:%d have no deal with", packet->m_head.cmd);
					break;
			}
		}
		else
		{
			slp.Start();
		}

	}
	return 0;
}
