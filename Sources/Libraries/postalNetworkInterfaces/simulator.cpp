// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "simulator.h"
#include "common/waitable_queue.h"
#include "defaultLogs/library.include.h"

namespace postal
{
	mt::waitable_queue<ConnectionSimulator*> localConnections;
	

	
	
	
	ConnectionSimulator::~ConnectionSimulator()
	{
		static std::mutex mtx;
		localConnections.replace(this, nullptr);
	
		std::unique_lock<std::mutex> lock(mtx);
		if (correspond)
		{			
			std::unique_lock<std::mutex> lock(correspond->monitor);
			correspond->correspond = nullptr;						
			correspond = nullptr;
		}		
	}

	bool ConnectionSimulator::connect(const NetworkAddress& addr)
	{
		correspond = new ConnectionSimulator();

		name = "client-side";
		correspond->name = "server-side";
		correspond->correspond = this;
		localConnections.send(correspond);
		return true;
	}
	
	std::size_t ConnectionSimulator::sendmsg(const char* msgdata, std::size_t msgsize) const
	{		
		std::unique_lock<std::mutex> lock(monitor);
		if (correspond)
		{
			std::unique_lock<std::mutex> lock(correspond->monitor);
			correspond->messages.emplace_back(new stream::dmemstream());
			correspond->messages.back()->write(msgdata, msgsize);
			return msgsize;
 		}

		return 0;
	}

	std::size_t ConnectionSimulator::recvmsg(char* msgdata, std::size_t msgsize) const
	{
		std::unique_lock<std::mutex> lock(monitor);

		if (!messages.empty())
		{
			auto& message = messages.front();
			auto msz = message->size();
			ENFORCE(msgsize >= msz);

			message->read(msgdata, msz);
			messages.pop_front();

			return msz;
 		}

		return 0;
	}


	
	
	
	std::atomic_uint gindex;
	ConnectionsSimulator::ConnectionsSimulator()
	{
		index = gindex++;
		localConnections.restart();
	}

	bool ConnectionsSimulator::start(unsigned short port)
	{
		return true;
	}

	void ConnectionsSimulator::stop(std::thread& joinable)
	{
		localConnections.cancel();
		if (joinable.joinable())
		{
			joinable.join();
		}
	}

	ConnectionSimulator* ConnectionsSimulator::incoming()
	{
		ConnectionSimulator* res = nullptr;
		localConnections.receive(res);

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