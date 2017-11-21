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