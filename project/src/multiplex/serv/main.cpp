#include "net_manager.h"
#include "sleeper.h"
#include "net_packet.h"
#include "sync_log.h"

using namespace Common::NetReactor;
using namespace Common::Thread;
using namespace Common::Time;
using namespace Common::Log;

int main()
{
	char response[1024] = {0};
	NetManager g_net_manager;
	NetPacket * packet = NULL;
	if ( -1 == g_net_manager.Work())
	{
		return -1;
	}
	Sleeper slp(0, 5000);
	while(true)
	{
		packet = g_net_manager.GetRecvNetPacket();
		if ( NULL != packet )
		{
			SyncLog::LOG(INFO,"Some Client Say:%s", packet->m_buff);
			sprintf(response, "Server Welcome You! %s", packet->m_buff);
			packet->Write(response, 1024);
			g_net_manager.AddSendNetPacket(packet);
			::memset(response, 0 , 1024);
			SyncLog::LOG(INFO,"!!!!! respones:%s", response);
		}
		else
			g_net_manager.Wait();
	}
	return 0;
}
