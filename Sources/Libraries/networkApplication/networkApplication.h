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