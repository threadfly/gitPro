#include "client.h"

#define CLIENT_NUM 100
int main()
{
	BlockClient* client[CLIENT_NUM];
	string ip("192.168.180.35");
	for(int i = 0; i < CLIENT_NUM; ++i)
	{	

		client[i] =  new BlockClient(i, ip, 5888);
		client[i]->Start();
	}

	for(int i = 0; i < CLIENT_NUM; ++i)
	{
		client[i]->Join();
		delete client[i];
	}

	return 0;
}
