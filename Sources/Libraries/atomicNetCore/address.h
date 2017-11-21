#pragma once

#include <stdint.h>
#include <iostream>
#include <vector>

#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#else
#include <winsock2.h>
#endif // WIN32


namespace net
{
	struct address4
	{
	public:
		address4();
		address4(const char* ip);
		address4(const char* ip, uint16_t port);
		address4(uint8_t ip0, uint8_t ip1, uint8_t ip2, uint8_t ip3, uint16_t port);
		address4(const address4& addr, unsigned short port);
		address4(const sockaddr_in& addr);
		address4(const addrinfo& info);

		static address4 localhost(uint16_t port);
		static bool parse(const char* addr, uint8_t& ip0, uint8_t& ip1, uint8_t& ip2, uint8_t& ip3, uint16_t& port);

		bool set(const char* ip);
		void set(uint8_t ip0, uint8_t ip1, uint8_t ip2, uint8_t ip3, uint16_t port);
		void get(uint8_t& ip0, uint8_t& ip1, uint8_t& ip2, uint8_t& ip3, uint16_t& port) const;

		address4 broadcast(uint16_t port) const;


		friend bool				operator==	(const address4& a1, const address4& a2)	{ return a1.ip() == a2.ip() && a1.port() == a2.port();	}
		friend bool				operator!=	(const address4& a1, const address4& a2)	{ return !operator==(a1, a2);	}
		friend bool				operator<	(const address4& a1, const address4& a2)	{ return (a1.ip() < a2.ip()) || (a1.ip()==a2.ip() && a1.port() < a2.port());	}
		friend std::ostream&	operator<<	(std::ostream& os, const address4& ip)		{ return os << ((ip.ip()>>0)&0xFF) << "." << ((ip.ip()>>8)&0xFF) << "." << ((ip.ip()>>16)&0xFF) << "." << ((ip.ip()>>24)&0xFF) << ":" << ip.port(); }
		
		bool isLocal() const;

		uint32_t ip() const;
		uint16_t port() const;
		inline const sockaddr_in& address() const { return addrin; }

		std::string string() const;
		std::string addr_str() const;
		std::string port_str() const;

	private:
		sockaddr_in addrin;
	};

	struct host_info
	{
		host_info();
		std::string name;
		std::vector<address4> addresses;

		std::pair<bool, address4> getLAN() const;

		std::string description() const;
	};
}

std::ostream& operator<<(std::ostream& os, const sockaddr_in& ip);