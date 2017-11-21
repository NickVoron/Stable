#include "address.h"

namespace udtnet
{
	//
	//
	//
	Address::Address()	
	{ 
		ip = port = -1;	
	}

	Address::Address(unsigned long _ip, unsigned short _port) 
		: ip(_ip), port(_port)
	{

	}

	Address::Address(unsigned char ip0, unsigned char ip1, unsigned char ip2, unsigned char ip3, unsigned short _port) 
		:port(_port)
	{
		ip = (ip0<<0) | (ip1<<8) | (ip2<<16) | (ip3<<24);
	}

	Address::Address(const Address& addr, unsigned short port):ip(addr.ip), port(addr.port)
	{

	}

	Address::Address(const sockaddr_in& addr)
	{
		ip = (addr.sin_addr.S_un.S_un_b.s_b1<<0) | (addr.sin_addr.S_un.S_un_b.s_b2<<8) | (addr.sin_addr.S_un.S_un_b.s_b3<<16) | (addr.sin_addr.S_un.S_un_b.s_b4<<24);
		port = ntohs(addr.sin_port);
	}

	void Address::flush(sockaddr_in& addr) const
	{
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.S_un.S_addr = ip;
	}

	bool Address::isLocal() const
	{
		bool res0 = ((ip>>0)&0xFF) == 192 && ((ip>>8)&0xFF) == 168;
		bool res1 = ((ip>>0)&0xFF) == 172 && (((ip>>8)&0xFF) >= 16 && ((ip>>8)&0xFF) <= 31);
		bool res2 = ((ip>>0)&0xFF) == 10 && ((ip>>8)&0xFF) == 0;
		
		return res0 || res1 || res2;
	}
}