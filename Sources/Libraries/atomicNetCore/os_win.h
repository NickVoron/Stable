// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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