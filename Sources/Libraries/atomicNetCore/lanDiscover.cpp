// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "lanDiscover.h"
#include "stuff/library.include.h"

namespace net
{
	lan_discover_udp_node::lan_discover_udp_node() : sock(ProtocolType::UDP, 55566, false)
	{
		
	}

	void lan_discover_udp_node::request(uint16_t port) const
	{
		static int i = 0;
		host_info host;
		auto lanaddr = host.getLAN();

        i++;

		if (lanaddr.first)
		{
			sock.send(lanaddr.second.broadcast(port), Request());
		}					
	}

	std::vector<net::address4> lan_discover_udp_node::result() const
	{
		std::vector<net::address4> result;
		net::address4 addr;
		Answer answ;
		while (sock.receive((char*)&answ, sizeof(answ), addr))
		{
			if (std::find_if(host.addresses.begin(), host.addresses.end(), [ip = addr.ip()](auto& addr){ return addr.ip() == ip; }) == host.addresses.end())
			{

				result.emplace_back(addr);
			}			
		}

		return result;
	}

	void lan_discover_udp_node::answer() const
	{
		net::address4 addr;
		Request req, ethalon;
		while (sock.receive((char*) &req, sizeof(req), addr))
		{
			if (memcmp(&req, &ethalon, sizeof(req)) == 0)
			{
				LOG_MSG("answer to: " << addr.addr_str());
				sock.send(addr, Answer());
				return;
			}			
		}
	}

	
	
	
	void udp_packets_logger::listen(uint16_t port)
	{
		sock = std::make_unique<net::Socket>(ProtocolType::UDP, port, false);
	}

	void udp_packets_logger::read()
	{
		if (sock)
		{
			net::address4 addr;
			char data[65535];
			while (auto rcv = sock->receive((char*) &data[0], sizeof(data), addr))
			{
				LOG_MSG(str::spaced("udp_packets_logger::read() : on local port:", sock->port(), rcv, "bytes from:", addr).str());
			}
		}		
	}

	
	
	
	udp_packets_sender::udp_packets_sender() : sock(ProtocolType::UDP, false)
	{

	}

	void udp_packets_sender::send(const net::address4& addr, uint16_t size)
	{
		char buffer[65536];
		sock.send(addr, &buffer[0], size);
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