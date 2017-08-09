// Copyright (C) 2015-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "stuff/library.include.h"

namespace udtnet
{							
	struct Address
	{
	public:
		Address();
		Address(unsigned long ip, unsigned short port);
		Address(unsigned char ip0, unsigned char ip1, unsigned char ip2, unsigned char ip3, unsigned short port);
		Address(const Address& addr, unsigned short port);
		Address(const sockaddr_in& addr);

		friend bool operator==(const Address& a1, const Address& a2)	{	return a1.ip==a2.ip && a1.port==a2.port;	}
		friend bool operator!=(const Address& a1, const Address& a2)	{	return !operator==(a1, a2);	}
		friend bool operator<(const Address& a1, const Address& a2)		{	return (a1.ip < a2.ip) || (a1.ip==a2.ip && a1.port < a2.port);	}
		friend std::ostream& operator<<(std::ostream& os, const Address& ip) { return os << ip.addr_str() << ":" << ip.port_str(); }

		void flush(sockaddr_in& res) const;
		bool isLocal() const;

		std::string addr_str() const { return str::stringize(((ip >> 0) & 0xFF), ".", ((ip >> 8) & 0xFF), ".", ((ip >> 16) & 0xFF), ".", ((ip >> 24) & 0xFF)).str(); }
		std::string port_str() const { return str::stringize(port).str(); }

	private:
		unsigned long ip;
		unsigned short port;
	};
}



// Copyright (C) 2015-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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