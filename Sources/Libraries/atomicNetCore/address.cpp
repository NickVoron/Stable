// Copyright (C) 2017-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "address.h"
#include "os_win_err.h"

#include "common/stringize.h"

#ifdef WIN32
#include <ws2ipdef.h>
#include <WS2tcpip.h>
#elif defined(__ANDROID__)
#include <sys/socket.h>
#include <sys/utsname.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <errno.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#elif defined(__APPLE__)
#include <sys/socket.h>
#include <sys/utsname.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <unistd.h>
#include <errno.h>
#endif 


namespace net
{
	
	
	
	address4::address4()
	{ 
		memset(&addrin, 0, sizeof(addrin));
	}

	bool address4::parse(const char* addr, uint8_t& ip0, uint8_t& ip1, uint8_t& ip2, uint8_t& ip3, uint16_t& port)
	{
		ip0 = ip1 = ip2 = ip3 = 0;
		port = 0;

		auto count = sscanf(addr, "%hhu.%hhu.%hhu.%hhu:%hu", &ip0, &ip1, &ip2, &ip3, &port);
		return (count >= 4);
	}
    
    bool address4::valid_str(const char *addr)
    {
        uint8_t ip0, ip1, ip2, ip3;
        uint16_t port;
        return parse(addr, ip0, ip1, ip2, ip3, port);
    }

	address4 address4::localhost(uint16_t port)
	{
		return address4(127, 0, 0, 1, port);
	}

	address4::address4(const char* ip)
	{
		set(ip);
	}

	address4::address4(const char* _ip, uint16_t _port)
	{
		memset(&addrin, 0, sizeof(addrin));
		addrin.sin_family = AF_INET;
		addrin.sin_addr.s_addr = inet_addr(_ip);
		addrin.sin_port = htons(_port);		
	}

	address4::address4(uint8_t ip0, uint8_t ip1, uint8_t ip2, uint8_t ip3, uint16_t _port)
	{
		set(ip0, ip1, ip2, ip3, _port);
	}

 	address4::address4(const address4& addr, uint16_t _port)
 	{
		addrin = addr.addrin;
		addrin.sin_port = htons(_port);
 	}

	address4::address4(const sockaddr_in& addr) : addrin(addr)
	{
	
	}

	address4::address4(const addrinfo& info) : address4(*(struct sockaddr_in*)info.ai_addr)
	{

	}

	bool address4::set(const char* ipstr)
	{
		uint8_t ip[4];
		uint16_t port;
		
		bool res = true;
		if ((res = parse(ipstr, ip[0], ip[1], ip[2], ip[3], port)))
		{
			set(ip[0], ip[1], ip[2], ip[3], port);
		}

		return res;
	}

	void address4::set(uint8_t ip0, uint8_t ip1, uint8_t ip2, uint8_t ip3, uint16_t _port)
	{
		memset(&addrin, 0, sizeof(addrin));
		addrin.sin_family = AF_INET;
#ifdef WIN32
		addrin.sin_addr.S_un.S_addr = (ip0 << 0) | (ip1 << 8) | (ip2 << 16) | (ip3 << 24);
#else
		addrin.sin_addr.s_addr = (ip0 << 0) | (ip1 << 8) | (ip2 << 16) | (ip3 << 24);
#endif
		addrin.sin_port = htons(_port);
	}
	
	bool address4::isLocal() const
	{
		auto ipvalue = ip();
		auto ip1 = ((ipvalue >> 0) & 0xFF);
		auto ip2 = ((ipvalue >> 8) & 0xFF);
		return	(ip1 == 192 && ip2 == 168 ) ||
				(ip1 == 172 && (ip2 >= 16 && ip2 <= 31)) ||
				(ip1 == 10  && ip2 == 0);

	}

	uint32_t address4::ip() const
	{
#ifdef WIN32
		return	(addrin.sin_addr.S_un.S_un_b.s_b1 << 0) | 
				(addrin.sin_addr.S_un.S_un_b.s_b2 << 8) | 
				(addrin.sin_addr.S_un.S_un_b.s_b3 << 16) | 
				(addrin.sin_addr.S_un.S_un_b.s_b4 << 24);
#else
		return addrin.sin_addr.s_addr;
#endif
	}

	void address4::get(uint8_t& ip0, uint8_t& ip1, uint8_t& ip2, uint8_t& ip3, uint16_t& port) const
	{
		auto ipv = ip();
		std::tie(ip0, ip1, ip2, ip3, port) = std::make_tuple((uint8_t) ((ipv >> 0) & 0xFF), (uint8_t) ((ipv >> 8) & 0xFF), (uint8_t) ((ipv >> 16) & 0xFF), (uint8_t) ((ipv >> 24) & 0xFF), this->port());
	}

	address4 address4::broadcast(uint16_t port) const
	{
		uint8_t ip[4];
		uint16_t prt;
		get(ip[0], ip[1], ip[2], ip[3], prt);
		return net::address4(ip[0], ip[1], ip[2], 255, port);
	}

	uint16_t address4::port() const
	{
		return ntohs(addrin.sin_port);
	}

	std::string address4::string() const
	{
		return str::stringize(addr_str(), ":", port_str());
	}

	std::string address4::addr_str() const
	{
		auto ipv = ip();
		return str::pointed(((ipv >> 0) & 0xFF), ((ipv >> 8) & 0xFF), ((ipv >> 16) & 0xFF), ((ipv >> 24) & 0xFF)).str();
	}

	std::string address4::port_str() const { return str::stringize(port()).str(); }

	host_info::host_info()
	{
        struct ::ifaddrs* myaddrs[128];
        memset(myaddrs, 0, std::size(myaddrs));

        struct ::ifaddrs *ifa = nullptr;
		void *in_addr;
		
        if (::getifaddrs(&myaddrs[0]) == 0)
		{
			for (ifa = myaddrs[0]; ifa != NULL; ifa = ifa->ifa_next)
			{
				if (ifa->ifa_addr == NULL)
					continue;
				if (!(ifa->ifa_flags & IFF_UP))
					continue;

				switch (ifa->ifa_addr->sa_family)
				{
				case AF_INET:
				{
					struct sockaddr_in *s4 = (struct sockaddr_in *)ifa->ifa_addr;
					addresses.emplace_back(*s4);
					break;
				}

				case AF_INET6:
				{
					struct sockaddr_in6 *s6 = (struct sockaddr_in6 *)ifa->ifa_addr;
					in_addr = &s6->sin6_addr;
					break;
				}

				default:
					continue;
				}
			}
		}
	}

	std::string host_info::description() const
	{
		str::stringize result;
		int i = 0;
		for (auto& local_address : addresses)
		{
			result("local adapter address ", i++, ": ", local_address.addr_str(), "\n");
		}

		return result.str();
	}

	std::pair<bool, address4> host_info::getLAN() const
	{
		for (auto& local_address : addresses)
		{
			if (local_address.isLocal())
			{
				return std::make_pair(true, local_address);
			}
		}

		return std::make_pair(false, address4::localhost(0));
	}
}

std::ostream& operator<<(std::ostream& os, const sockaddr_in& ip) { return os << net::address4(ip); }




// Copyright (C) 2017-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.