// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "udp_impl.h"
#include "defaultLogs/library.include.h"


namespace postal
{
	UDPConnection::UDPConnection()
	{
	}

	UDPConnection::UDPConnection(const sockaddr_in& insock)
	{
		address = net::address4(insock);
		sock = std::make_unique<net::Socket>(net::ProtocolType::UDP, false);
	}

	UDPConnection::~UDPConnection()
	{
		close();
	}

	bool UDPConnection::connect(const NetworkAddress& addr)
	{
		LOG_MSG("connect to:" << addr.address.string());
		address = addr.address;
		sock = std::make_unique<net::Socket>(net::ProtocolType::UDP, false);
		return true;
	}

	void UDPConnection::close()
	{
		std::unique_lock<std::mutex> lock(sockmtx);
		sock = nullptr;
	}

	void UDPConnection::sendmsg(const char* msgdata, std::size_t msgsize) const
	{	
		std::unique_lock<std::mutex> lock(sockmtx);
		if (sock && msgsize > 0)
		{
			sock->send(address, msgdata, msgsize);
		}		
	}

	std::size_t UDPConnection::recvmsg(char* msgdata, std::size_t msgsize) const
	{
		std::unique_lock<std::mutex> lock(sockmtx);

		return sock ? sock->receive(msgdata, msgsize) : 0;
	}


	
	
	
	UDPConnections::~UDPConnections()
	{
		close();
	}

	bool UDPConnections::start(uint16_t port)
	{
		serv = std::make_unique<net::Socket>(net::ProtocolType::UDP, port, true);
		return true;
	}

	void UDPConnections::close()
	{		
		serv = nullptr;
	}

	void UDPConnections::stop(std::thread& joinable)
	{
		close();
		joinable.join();
	}

	UDPConnection* UDPConnections::incoming()
	{
		std::unique_lock<std::mutex> lock(sockmtx);
		
		UDPConnection* res = nullptr;
		struct sockaddr_in dest;
		int data;
		while (true)
		{
			if (serv->receive((char*) &data, sizeof(data), dest))
			{
				return new UDPConnection(dest);
			}
		}		
		
		return res;
	}
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