// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "os.h"

#include <memory>
#include <vector>

#include "address.h"


namespace net
{
	struct lan_discover_udp_node
	{
		struct Request	{ uint8_t data[4] = {'A', 'C', 'K', 'K'}; };
		struct Answer	{ uint8_t data[4] = { 'A', 'N', 'S', 'W' }; };

		lan_discover_udp_node();

		void request(uint16_t port) const;
		std::vector<net::address4> result() const;
		void answer() const;

		net::Socket sock;
		host_info host;
	};

	struct udp_packets_logger
	{
		void listen(uint16_t port);
		void read();

		std::unique_ptr<net::Socket> sock;
	};

	struct udp_packets_sender
	{
		udp_packets_sender();
		void send(const net::address4& addr, uint16_t size);
		net::Socket sock;
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