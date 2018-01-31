// Copyright (C) 2017-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <list>
#include <atomic>

#include "stream/library.include.h"

#include "address.h"

namespace postal
{
	struct ConnectionSimulator
	{
		~ConnectionSimulator();

		bool connect(const NetworkAddress& addr);
		std::size_t sendmsg(const char* msgdata, std::size_t msgsize) const;
		std::size_t recvmsg(char* msgdata, std::size_t msgsize) const;

		std::string name = "unknown-side";
		ConnectionSimulator* correspond = nullptr;

		mutable std::mutex monitor;
		mutable std::list<std::unique_ptr<stream::dmemstream>> messages;
	};

	struct ConnectionsSimulator
	{
		template<class Function>
		bool accept(Function&& function)
		{
			if (auto connection = incoming())
			{
				function(connection);
				return true;
			}			
			return false;
		}

		ConnectionsSimulator();

		bool start(unsigned short port);
		void stop(std::thread& joinable);
			
		ConnectionSimulator* incoming();
		std::atomic_uint index;
	};
}



// Copyright (C) 2017-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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