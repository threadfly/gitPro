#ifndef _NET_PACKET
#define _NET_PACKET

namespace Common
{

namespace Net
{
	class NetPacket
	{
	public:
		NetPacket();
		~NetPacket(){};

		int Write(char * src, int len);

		int Read(char * buff, int len);
	
	public:
		int  m_buff_len;
		//TODO ��������ǲ��ǲ���... Ӧ������һ����̬�����İ�...
		char buff[NET_PACKET_BUFF_SIZE] m_buff;
	};

}

}

#endif
