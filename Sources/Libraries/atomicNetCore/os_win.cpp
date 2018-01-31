// Copyright (C) 2017-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "os_win.h"
#include "os_win_err.h"

#ifdef WIN32
#include <WS2tcpip.h>
#else
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

namespace net
{
#ifdef WIN32
	std::ostream& operator<<(std::ostream& os, const WSADATA& data)
	{
		return os;
	}
#endif

	void init()
	{
#ifdef WIN32
		WORD wVersion = MAKEWORD(2, 2);
		WSADATA wsaData;
		error(WSAStartup (wVersion, &wsaData));
#endif
	}

	void release()
	{
#ifdef WIN32
		error(WSACleanup());
#endif
	}

	socket_type socket_tcp() { return ::socket (PF_INET, SOCK_STREAM, IPPROTO_TCP); }
	socket_type socket_udp() { return ::socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);  }

	socket_type socket(ProtocolType pt)
	{
		switch(pt)
		{
		case ProtocolType::TCP: return socket_tcp();
		case ProtocolType::UDP: return socket_udp();
		}

		return SOCKET_ERROR;
	}

	void closesocket(socket_type s)
	{
#ifdef WIN32
		::closesocket(s);
#else
		
#endif
	}
	
	
	
	
	Socket::Socket(ProtocolType pt, bool blocked, bool broadcast, bool localhost) : blocked(blocked), localhost(localhost)
	{
		sock = socket(pt);
		error_socket(sock);
		
#ifdef WIN32
		u_long mode = blocked ? 0 : 1;
		ioctlsocket(sock, FIONBIO, &mode);
#else
		fcntl(sock, F_SETFL, O_NONBLOCK);
#endif

		error_socket(sock);
	}

	Socket::~Socket()
	{
		closesocket(sock);
	}

	
	
	Socket::Socket(ProtocolType protocol, uint16_t port, bool blocked, bool broadcast, bool localhost) : Socket(protocol, blocked, broadcast, localhost)
	{
		for (uint16_t p = 0; p < 16; ++p)
		{
			if(bindToPort(port + p))
				break;
		}		
	}

	bool Socket::bindToPort(uint16_t port)
	{
		host_info hostInfo;
		auto addr = net::address4::localhost(port);
		if(auto lanaddr = hostInfo.getLAN(); lanaddr.first && !localhost)
		{
			addr = net::address4(lanaddr.second, port);
		}

		LOG_MSG("Socket::bindToPort: " << addr.string());

		auto bindres = bind(sock, (struct sockaddr*)&addr.address(), sizeof(addr.address()));
		if (SOCKET_ERROR == bindres)
		{
			error_socket(bindres);
			return false;
		}		

		return true;
	}

	bool Socket::send(const address4& addr, const char* data, std::size_t dataSize) const
	{
		return error_socket(sendto(sock, data, dataSize, 0, (struct sockaddr*)&addr.address(), sizeof(addr.address())));
	}

	std::size_t Socket::receive(char* data, std::size_t dataSize, sockaddr_in& dest) const
	{		
		socklen_t destlen = (int)sizeof(dest);
		int res = 0;
		res = recvfrom(sock, data, dataSize, 0, (struct sockaddr *)&dest, &destlen);
	
		if (res == SOCKET_ERROR)
		{
			error_socket(res);
		}

		return (res != SOCKET_ERROR) ? res : 0;
	}

	std::size_t Socket::receive(char* data, std::size_t dataSize) const
	{
		struct sockaddr_in dest;
		return receive(data, dataSize, dest);
	}

	std::size_t Socket::receive(char* data, std::size_t dataSize, address4& addr) const
	{
		struct sockaddr_in dest;
		auto res = receive(data, dataSize, dest);
		addr = net::address4(dest);
		return res;
	}

	uint16_t Socket::port() const
	{
		return address().port();
	}

	address4 Socket::address() const
	{
		struct sockaddr_in sin;
		socklen_t addrlen = sizeof(sin);
		if (getsockname(sock, (struct sockaddr *)&sin, &addrlen) == 0 && sin.sin_family == AF_INET && addrlen == sizeof(sin))
		{
			return address4(sin);
		}

		return address4();
	}

	std::string Socket::addr_str() const
	{
		return address().addr_str();
	}

	std::string Socket::port_str() const
	{
		return address().port_str();
	}

	std::string Socket::string() const
	{
		return address().string();
	}
}




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