#include <set>
#include <signal.h>

#include "net_manager.h"
#include "sleeper.h"
#include "net_packet.h"
#include "net_connect_socket.h"
#include "net_reactor.h"
#include "sync_log.h"

// current dir
#include "config.h"

#include "cmd.pb.h"
#include "cs.pb.h"

using namespace Common::NetReactor;
using namespace Common::Thread;
using namespace Common::Time;
using namespace Common::Log;

//protocol
using namespace cs;

using namespace std;

int main()
{
	//char response[1024] = {0};
	std::set<uint32_t> ids;
	signal(SIGPIPE, SIG_IGN);

	NetManager g_net_manager;
	NetPacket * packet = NULL;
	if ( -1 == g_net_manager.WorkAsClient())
	{
		return -1;
	}

	Sleeper slp(0, 20000);

	LoadServerConfig loadConf;
	loadConf.LoadConfig("../config/server.xml");

	ConnectSocket * client = new ConnectSocket(&g_net_manager);
	if ( -1 == client->CreateTcpClient(loadConf.m_server_ip , loadConf.m_server_port))
	{
		SyncLog::LOG(EROR, "Client Connect Error !!!");
		return -1;
	}

	uint32_t clientid = client->GetId();
	ids.insert(clientid);
	g_net_manager.GetReactor()->RegisterHandler(client);
	while(true)
	{
		packet = g_net_manager.GetRecvNetPacket();
		if ( NULL != packet )
		{
			std::set<uint32_t>::iterator it = ids.find(packet->m_handler_id);
			if (it == ids.end())
			{
				SyncLog::LOG(EROR, "it == ids.end()");
				continue;
			}

			switch (packet->m_head.cmd)
			{
				case ID_S2C_Login:
				{
					S2C_Login login;
					string message(packet->m_buff, packet->m_buff_len);
					login.ParseFromString(message);
					SyncLog::LOG(INFO, "Server ret:%d", login.ret());
					
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
			C2S_Login c2s_login;
			c2s_login.set_id(5889);
			c2s_login.set_name("wff");
			string req;
			c2s_login.SerializeToString(&req);
			
			InnerPacketHead ihead;
			ihead.length = req.length();
			ihead.sid = 1;
			ihead.uid = 0;
			ihead.cmd = ID_C2S_Login;
			ihead.hid = clientid;

			g_net_manager.SendMessage(ihead, req.c_str());

			SyncLog::LOG(INFO, "Client Req id:%d name:%s hid:%d cmd:%d", c2s_login.id(), c2s_login.name().c_str(), clientid, ID_C2S_Login);

			//slp.Start();
		}

	}
	return 0;
}
