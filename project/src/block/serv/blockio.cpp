#include<iostream>
#include "server.h"
using namespace std;

int main()
{	
	string ip("192.168.180.35");
	BlockServer server( ip, 5888);
	server.Start();
	if (false == server.Join())
	{
		std::cout<<"server join error" << std::endl;
	}
	return 0;
}
