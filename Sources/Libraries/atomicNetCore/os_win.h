#pragma once

#ifdef WIN32
#pragma comment (lib, "ws2_32.lib")
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#endif

#include "address.h"
#include "ifaddrs.h"

namespace net
{
	enum class ProtocolType : uint8_t
	{
		TCP,
		UDP
	};

#ifdef WIN32
	typedef SOCKET socket_type;
#else
	typedef int socket_type;
#define INVALID_SOCKET  (::net::socket_type)(~0)
#endif

	void init();
	void release();

	//
	struct Socket 
	{
	public:
		Socket(ProtocolType protocol, bool blocked);
		Socket(ProtocolType protocol, uint16_t port, bool blocked);
		~Socket();

		bool send(const address4& addr, const char* data, std::size_t dataSize) const;

		template<class Data>
		void send(const address4& addr, const Data& data) const
		{
			send(addr, (const char*)&data, sizeof(data));
		}

		std::size_t receive(char* data, std::size_t dataSize) const;
		std::size_t receive(char* data, std::size_t dataSize, sockaddr_in& sock) const;
		std::size_t receive(char* data, std::size_t dataSize, address4& addr) const;

		uint16_t port() const;

	protected:

		bool bindToPort(uint16_t port);

		bool blocked;
		socket_type sock;
	};
}