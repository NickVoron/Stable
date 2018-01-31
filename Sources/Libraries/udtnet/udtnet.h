// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "defaultLogs/library.include.h"
#include "udt/library.include.h"
#include "stream/library.include.h"

#include "address.h"

#include <thread>
#include <mutex>

namespace udtnet
{
	static const int MAX_MSG_SIZE = 65535;

	typedef stream::simemstream<MAX_MSG_SIZE> IStream;
	typedef stream::smemstream<MAX_MSG_SIZE> IOStream;

	class Connection;
	struct DataDispatcher
	{
		virtual ~DataDispatcher() {}
		virtual void start(Connection& connection) = 0;
		virtual void recvmsg(IStream& ios) = 0;
		virtual void internalMessage(udtnet::IOStream& is) = 0;
	};

	class Connection : public Base::NonCopyable
	{
	public:
		Connection(DataDispatcher* disp = 0) { bindDispatcher(disp); }
		Connection(UDTSOCKET s, DataDispatcher* disp = 0) : sock(s) { bindDispatcher(disp); }
		~Connection();

		void bindDispatcher(DataDispatcher* disp) { dispatcher = disp; };
		bool connect(const Address& addr);
		void close();

		void dispatcherStart();
		void dispatch();
		void dispatch(IStream& ios);

		template<class T>
		void sendmsg(const T& data) const
		{
			sendmsg((const char*) &data, sizeof(data));
		}

		void sendmsg(const char* msgdata, int msgsize) const;
		void sendmsg(const mem::mem_desc& memblock) const;
		int recvmsg(char* msgdata, int msgsize) const;
		void internalMessage(udtnet::IOStream& is);

		bool operator< (const Connection& connection) const { return sock < connection.sock; }

	private:
		UDTSOCKET sock = 0;
		DataDispatcher* dispatcher = 0;
		bool exitDispatcher = false;
	};			

	class Connections : public std::list<std::unique_ptr<Connection>>
	{
	public:
		~Connections();
		bool server(unsigned short port, std::function<DataDispatcher*()> factory);
		void stop();
		void dispatch();
		void internalMessage(udtnet::IOStream& is);
	
	private:
		void addConnection(UDTSOCKET connect, DataDispatcher* dispatcher);

		std::mutex lockmtx;

		std::thread dispatcherThread;
		std::thread connections;

		UDTSOCKET serv = 0;
		bool exitConnection = false;
		bool exitDispatcher = false;
	};

	void init();
	void release();
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