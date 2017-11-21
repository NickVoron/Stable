#include "os_win.h"
#include "os_win_err.h"

#include "memory/library.include.h"
#include "defaultLogs/library.include.h"
#include "stuff/enforce.h"
#include "stuff/library.include.h"

#ifdef WIN32
#include <WS2tcpip.h>
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
		close(s);
#endif
	}
	
	//
	//
	//
	Socket::Socket(ProtocolType pt, bool blocked) : blocked(blocked)
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

	//
	//
	Socket::Socket(ProtocolType protocol, uint16_t port, bool blocked) : Socket(protocol, blocked)
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
		auto lanaddr = hostInfo.getLAN();
		if (lanaddr.first)
		{
			auto addr = net::address4(lanaddr.second, port);
	
			LOG_MSG("Socket::bindToPort: " << addr.string());
		
			auto bindres = bind(sock, (struct sockaddr*)&addr.address(), sizeof(addr.address()));
			if (SOCKET_ERROR == bindres)
			{
				error_socket(bindres);
				return false;
			}
		}
		else
		{
			LOG_MSG("Socket::bindToPort: incorrect network address");
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
		struct sockaddr_in sin;
		socklen_t addrlen = sizeof(sin);
		if (getsockname(sock, (struct sockaddr *)&sin, &addrlen) == 0 && sin.sin_family == AF_INET && addrlen == sizeof(sin))
		{
			return ntohs(sin.sin_port);
		}

		return 0;
	}
}
