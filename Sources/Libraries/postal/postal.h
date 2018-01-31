// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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
#include "connection.h"

#include "atomicNetCore/library.include.h"

namespace postal
{
	template<class AddressT, class MessageIOStreamT, class MessageIStreamT, class ClientTransitionsList, class ServerTransitionsList, class ConnectionInterface, class ConnectionsInterface, class EventType>
	struct ApplicationTypes
	{
		typedef AddressT Address;
		typedef MessageIStreamT MessageIStream;
		typedef MessageIOStreamT MessageIOStream;

		typedef postal::Connections<ConnectionInterface, ConnectionsInterface, ServerTransitionsList, Address, MessageIStream, MessageIOStream, EventType> ServerConnections;
		typedef postal::Connections<ConnectionInterface, ConnectionsInterface, ClientTransitionsList, Address, MessageIStream, MessageIOStream, EventType> ClientConnections;

		typedef typename ServerConnections::Connection ServerConnection;
		typedef typename ClientConnections::Connection ClientConnection;
		
		typedef typename ClientConnections::StateBase ClientStateBase;
		typedef typename ClientConnections::Start ClientStart;

		typedef typename ServerConnections::StateBase ServerStateBase;
		typedef typename ServerConnections::Start ServerStart;
	};
	
	template<class ApplicationBase, class ServerTransitionsList, class ClientTransitionsList>
	struct Application : public ApplicationBase
	{
		typedef typename ApplicationBase::Address Address;
		typedef typename ApplicationBase::MessageIOStream MessageIOStream;
		typedef typename ApplicationBase::MessageIStream MessageIStream;

		using ServerType = Server<
			Address, 
			MessageIOStream, 
			MessageIStream, 
			typename ApplicationBase::ServerConnection, 
			typename ApplicationBase::ServerConnections,
			typename ApplicationBase::ServerStateBase,
			ServerTransitionsList>;

		using ClientType = Client<
			Address, 
			MessageIOStream, 
			MessageIStream, 
			typename ApplicationBase::ClientConnection, 
			typename ApplicationBase::ClientStateBase,
			ClientTransitionsList>;

		std::unique_ptr<ServerType> server;
		std::unique_ptr<ClientType> client;

		~Application()	{ stop(); }

		template <class SideType, class Function>
		static bool startSide(std::unique_ptr<SideType>& side, const Address& param, Function&& func)
		{
			side = std::make_unique<SideType>();
			if (!side->start(param, std::forward<Function>(func)))
			{
				side = nullptr;
			}

			return side != nullptr;
		}


		template <class Function>
		bool startServer(uint16_t port, Function&& func)
		{
			stop();
			Address addr;
			addr.address = net::address4(Address::localhost().address, port);
			return startSide(server, addr, std::forward<Function>(func));
		}		

		template <class Function>
		bool startClient(const Address& address, Function&& func)
		{
			stop();
			return startSide(client, address, std::forward<Function>(func));
		}

		template <class ServerFunction, class ClientFunction>
		bool startHost(uint16_t port, ServerFunction&& serverFunc, ClientFunction&& clientFunc)
		{
			stop();

			bool res = true;

			auto localhost = Address::localhost(port);
			res &= startSide(server, localhost, std::forward<ServerFunction>(serverFunc));

			net::host_info hostInfo;
			auto lanaddr = hostInfo.getLAN();
			if (lanaddr.first)
			{
				localhost.address = net::address4(lanaddr.second, port);
			}
			
			res &= startSide(client, localhost, std::forward<ClientFunction>(clientFunc));

			return res;
		}

		void stop()
		{
			try
			{
				client = nullptr;
				server = nullptr;				
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