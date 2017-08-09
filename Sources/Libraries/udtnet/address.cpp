// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "address.h"

namespace udtnet
{
	
	
	
	Address::Address()	
	{ 
		ip = port = -1;	
	}

	Address::Address(unsigned long _ip, unsigned short _port) 
		: ip(_ip), port(_port)
	{

	}

	Address::Address(unsigned char ip0, unsigned char ip1, unsigned char ip2, unsigned char ip3, unsigned short _port) 
		:port(_port)
	{
		ip = (ip0<<0) | (ip1<<8) | (ip2<<16) | (ip3<<24);
	}

	Address::Address(const Address& addr, unsigned short port):ip(addr.ip), port(addr.port)
	{

	}

	Address::Address(const sockaddr_in& addr)
	{
		ip = (addr.sin_addr.S_un.S_un_b.s_b1<<0) | (addr.sin_addr.S_un.S_un_b.s_b2<<8) | (addr.sin_addr.S_un.S_un_b.s_b3<<16) | (addr.sin_addr.S_un.S_un_b.s_b4<<24);
		port = ntohs(addr.sin_port);
	}

	void Address::flush(sockaddr_in& addr) const
	{
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.S_un.S_addr = ip;
	}

	bool Address::isLocal() const
	{
		bool res0 = ((ip>>0)&0xFF) == 192 && ((ip>>8)&0xFF) == 168;
		bool res1 = ((ip>>0)&0xFF) == 172 && (((ip>>8)&0xFF) >= 16 && ((ip>>8)&0xFF) <= 31);
		bool res2 = ((ip>>0)&0xFF) == 10 && ((ip>>8)&0xFF) == 0;
		
		return res0 || res1 || res2;
	}
}



// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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