// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "utp_impl.h"
#include "defaultLogs/library.include.h"

#ifndef WIN32
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#pragma comment (lib, "ws2_32.lib")
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#include <wspiapi.h>
#endif

namespace postal
{
	extern std::unique_ptr<utp::core> kernel;

	UTPConnection::~UTPConnection()
	{
		close();
	}

	bool UTPConnection::connect(const NetworkAddress& addr)
	{
		if (!kernel)
		{
			kernel = std::make_unique<utp::core>(NetworkAddress::localhost().port());
		}
		
		sock = kernel->connect(addr.address);
		return true;
	}

	void UTPConnection::close()
	{
		std::unique_lock<std::mutex> lock(sockmtx);
		sock = nullptr;
	}

	NetworkAddress UTPConnection::address() const
	{
		sockaddr_in addrin;
		socklen_t addrlen = sizeof(addrin);
		utp_getpeername(sock, (sockaddr*)&addrin, &addrlen);
		NetworkAddress result;
		result.address = net::address4(addrin);
		return result;
	}
	

	std::size_t UTPConnection::sendmsg(const char* msgdata, std::size_t msgsize) const
	{	
		std::unique_lock<std::mutex> lock(sockmtx);
		std::size_t sended = 0;
 		if (sock && (msgsize > 0) )
 		{
			sended = utp_write(sock, (void*)msgdata, msgsize);
 		}		

		
		
		
		

		return sended;
	}

	std::size_t UTPConnection::recvmsg(char* msgdata, std::size_t msgsize) const
	{
		std::unique_lock<std::mutex> lock(sockmtx);
		std::size_t rs = 0;
		
		if (sock)
		{
			
			utp::socket_data* userData = (utp::socket_data*) utp_get_userdata(sock);
			if (userData)
			{
				auto& messages = userData->messages;
				if ((msgsize > 0) && !messages.empty())
				{
					auto& message = messages.front();
					rs = message.size();
                                        ENFORCE_LESS(rs, msgsize);
					memcpy(msgdata, &message[0], rs);
					messages.pop_front();
				}
			}
		}		
		
		return rs;
	}


	
	
	
	UTPConnections::~UTPConnections()
	{
		close();
	}

	bool UTPConnections::start(unsigned short port)
	{
		kernel = std::make_unique<utp::core>(port);
		if (!kernel->context)
		{
			kernel = nullptr;
		}

		return kernel != nullptr;
	}

	void UTPConnections::close()
	{		
		
		
		if (kernel)
		{
			kernel->incoming.cancel();
		}		
	}

	void UTPConnections::stop(std::thread& joinable)
	{
		close();		
		joinable.join();
	}

	UTPConnection* UTPConnections::incoming()
	{
		
		if (kernel)
		{
			utp_socket* sock = 0;
			if(kernel->incoming.receive(sock))
			{
				auto res = new UTPConnection();
 				res->sock = sock;
				LOG_WARNING("incoming:" << sock);
				return res;
			}
		}
		return nullptr;
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