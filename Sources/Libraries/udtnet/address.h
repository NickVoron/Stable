#pragma once

#include "stuff/library.include.h"

namespace udtnet
{							
	struct Address
	{
	public:
		Address();
		Address(unsigned long ip, unsigned short port);
		Address(unsigned char ip0, unsigned char ip1, unsigned char ip2, unsigned char ip3, unsigned short port);
		Address(const Address& addr, unsigned short port);
		Address(const sockaddr_in& addr);

		friend bool operator==(const Address& a1, const Address& a2)	{	return a1.ip==a2.ip && a1.port==a2.port;	}
		friend bool operator!=(const Address& a1, const Address& a2)	{	return !operator==(a1, a2);	}
		friend bool operator<(const Address& a1, const Address& a2)		{	return (a1.ip < a2.ip) || (a1.ip==a2.ip && a1.port < a2.port);	}
		friend std::ostream& operator<<(std::ostream& os, const Address& ip) { return os << ip.addr_str() << ":" << ip.port_str(); }

		void flush(sockaddr_in& res) const;
		bool isLocal() const;

		std::string addr_str() const { return str::stringize(((ip >> 0) & 0xFF), ".", ((ip >> 8) & 0xFF), ".", ((ip >> 16) & 0xFF), ".", ((ip >> 24) & 0xFF)).str(); }
		std::string port_str() const { return str::stringize(port).str(); }

	private:
		unsigned long ip;
		unsigned short port;
	};
}