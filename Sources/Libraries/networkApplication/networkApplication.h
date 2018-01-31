// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "types.h"
#include "states.h"
#include "server.h"
#include "client.h"

namespace network
{
	template<class ServerBaseState, class ServerTransitionsList, class ServerStartState, class ClientBaseState, class ClientTransitionsList, class ClientStartState>
	struct Application
	{
		typedef Server<ServerBaseState, ServerTransitionsList, ServerStartState> ServerType;
		typedef Client<ClientBaseState, ClientTransitionsList, ClientStartState> ClientType;

		ServerType* server = 0;
		ClientType* client = 0;


		~Application()	{ stop(); }

		template <class SideType, class SideParam, class Function>
		static bool startSide(SideType*& side, const SideParam& param, Function& func)
		{
			side = new SideType();
			bool started = side->start(param, [func](network::DataBuffer& buffer) { return func(buffer); });
			if (!started)
			{
				SAFE_DELETE(side);
			}

			return started;
		}


		template <class Function>
		bool startServer(unsigned short port, Function& func)
		{
			stop();
			return startSide(server, port, [func](network::DataBuffer& buffer) { return func(buffer.ios);	});
		}		

		template <class Function>
		bool startClient(const udtnet::Address& addr, Function& func)
		{
			stop();
			return startSide(client, addr, [func](network::DataBuffer& buffer) { return func(buffer.ios);	});
		}

		template <class ServerFunction, class ClientFunction>
		bool startHost(unsigned short port, ServerFunction& serverFunc, ClientFunction& clientFunc)
		{
			bool res = true;

			udtnet::Address addr(127, 0, 0, 1, port);
			res &= startSide(server, port, [serverFunc](network::DataBuffer& buffer) { return serverFunc(buffer.ios);	});
			res &= startSide(client, addr, [clientFunc](network::DataBuffer& buffer) { return clientFunc(buffer.ios);	});

			return res;
		}

		void stop()
		{
			try
			{
				SAFE_DELETE(server);
				SAFE_DELETE(client);
				Sleep(100);
			}
			catch (const std::exception&)
			{

			}
		}

		bool isServer() const { return server && !client; }
		bool isClient() const { return client && !server; }
		bool isHost() const { return server && client; }
		bool isRunning() const { return server || client; }
	};
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