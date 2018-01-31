// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <thread>
#include <mutex>

#include "address.h"

#include "stream/library.include.h"
#include "atomicNetCore/library.include.h"
#include "udt/library.include.h"

namespace postal
{
	struct UDPConnection
	{
		UDPConnection();
		UDPConnection(const sockaddr_in& insock);

		~UDPConnection();

		bool connect(const NetworkAddress& addr);
		void close();

		void sendmsg(const char* msgdata, std::size_t msgsize) const;
		std::size_t recvmsg(char* msgdata, std::size_t msgsize) const;

		std::string name = "unknown-side";

		mutable std::mutex sockmtx;
		std::unique_ptr<net::Socket> sock;
		net::address4 address;
	};

	struct UDPConnections
	{
		template<class Function>
		bool accept(Function&& function)
		{
			if (auto res = incoming())
			{
				function(res);
				return true;
			}
			return false;
		}

		~UDPConnections();
		bool start(uint16_t port);
		void stop(std::thread& joinable);
		void close();
			
		UDPConnection* incoming();

		mutable std::mutex sockmtx;
		std::unique_ptr<net::Socket> serv;
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