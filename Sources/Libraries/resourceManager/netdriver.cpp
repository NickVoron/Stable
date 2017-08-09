// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#ifdef NET_RES_DRIVER

#include "store.h"
#include "resource.h"

namespace Resources
{
	namespace client
	{
		
		
		
		void WorldDataExchange::enter(ClientConnection& connection)
		{
			LOG_MSG("CLIENT::WorldDataExchange::enter");
		}

		void WorldDataExchange::exit(ClientConnection& connection)
		{

		}

		void WorldDataExchange::internalMessage(MessageIOStream& is, ClientConnection& connection)
		{

		}

		void WorldDataExchange::externalMessage(MessageIStream& is, ClientConnection& connection)
		{
			Resources::Store::FactoryDict dict;
			is >> dict;

			auto event = parentFSM->empty_message();
			event.header.eventcls = postal::EVENT_CLASS::CHANGE_STATE_NO_WAIT;
			event.header.event = RESOURCES_EVENT_WORK;
			parentFSM->internal_post(event);
		}

		
		
		
		void Work::enter(ClientConnection& connection)
		{
			LOG_MSG("CLIENT::Work::enter");

		}

		void Work::exit(ClientConnection& connection)
		{

		}

		void Work::internalMessage(MessageIOStream& is, ClientConnection& connection)
		{
			LOG_MSG("CLIENT::Work::internalMessage()");
			Resources::ResourceNetQueries queries;
			is >> queries;

			if (!queries.empty())
			{
				for (auto& query : queries)
				{
					LOG_MSG("resource query: " << query.fileName.c_str() << " target: " << query.target);
				}

				LOG_MSG("raw sizeof: " << queries.size() * sizeof(queries[0]));

				postal::SendEventMessage::SEND(RESOURCES_EVENT_WORK, is, queries, connection);
			}
		}

		void Work::externalMessage(MessageIStream& is, ClientConnection& connection)
		{
			LOG_MSG("CLIENT::Work::externalMessage()");

			Resource* target;
			int platformID;
			int resourceTypeID;

			is >> target >> platformID >> resourceTypeID;
			LOG_MSG("resource result: target: " << target);

			
			{
				target->Clear();
				target->LoadCompiled(is);
			}
		}
	}

	
	
	
	namespace server
	{
		void WorldDataExchange::enter(ServerConnection& connection)
		{
			LOG_MSG("SERVER::WorldDataExchange::enter");
			Resources::Store::FactoryDict dict;
			Resources::Store::dictionary(dict);

			postal::SendEventMessage::SEND(RESOURCES_EVENT_DATA_EXCHANGE, dict, connection);
		}

		void WorldDataExchange::exit(ServerConnection& connection)
		{

		}

		void WorldDataExchange::internalMessage(MessageIOStream& is, ServerConnection& connection)
		{

		}

		void WorldDataExchange::externalMessage(MessageIStream& is, ServerConnection& connection)
		{

		}


		
		
		
		void Work::enter(ServerConnection& connection)
		{
			LOG_MSG("SERVER::Work::enter");
		}

		void Work::exit(ServerConnection& connection)
		{

		}

		void Work::internalMessage(MessageIOStream& is, ServerConnection& connection)
		{
			LOG_MSG("SERVER::Work::internalMessage");
			for (auto& cq : clientQueries)
			{
				for (auto& query : cq.second)
				{
					executeQuery(query, *cq.first);
				}
			}
		}

		void Work::externalMessage(MessageIStream& is, ServerConnection& connection)
		{
			LOG_MSG("SERVER::Work::externalMessage");
			ResourceNetQueries queries;
			is >> queries;

			for (auto& query : queries)
			{
				LOG_MSG("server receive query : " << query.fileName.c_str() << " target: " << query.target);
				executeQuery(query, connection);
				clientQueries[&connection].push_back(query);
			}
		}

		void Work::executeQuery(const Resources::ResourceNetQuery& query, ServerConnection& connection)
		{
			postal::SendEventMessage::SEND_FUNC(RESOURCES_EVENT_WORK, [&query](MessageIOStream& ios)
			{
				ios << query.target << query.platformID << query.resourceTypeID;
				Resources::compile(query.platformID, query.resourceTypeID, query.fileName, ios);
			}, connection);
		}

	}


	
	
	
	HostApplication::HostApplication()
	{

	}

	void HostApplication::net_host(uint16_t port)
	{
		startHost(port,
			[](MessageIOStream& ios)
		{
			if (Store::ReloadIfUpdate())
			{
				ios << 0;
				return true;
			}

			return false;
		},
			[this](MessageIOStream& ios)
		{
			ResourceNetQueries resqueries;
			ResourceNetQuery query;
			while (queries.try_receive(query))
			{
				resqueries.push_back(query);
			}

			if (!resqueries.empty())
			{
				ios << resqueries;
			}

			return !resqueries.empty();
		});
	}

	void HostApplication::net_server(uint16_t port)
	{
		launch_named_thread("ResourceServerStarter", [this, port]
		{
			bool started = false;
			while (!started)
			{
				started = startServer(port, [](MessageIOStream& ios)
				{
					if (Store::ReloadIfUpdate())
					{
						ios << 0;
						return true;
					}

					return false;
				});

				if (started)
				{
					launch_infinite_thread("ResourceServerUpdater", 5, [this]
					{
						if (server)
						{
							server->process();
							server->updateFSM();
						}

					}).detach();
				}
				else
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(2000));
					LOG_MSG("try to start ResourceServer");
				}
			}

		}).detach();
	}

	std::mutex clientmtx;

	void HostApplication::net_client(const Address& address)
	{
		launch_named_thread("ResourceClientStarter", [this, address]
		{
			bool started = false;
			while (!started)
			{
				{
					std::unique_lock<std::mutex> lock(clientmtx);
					startClient(address, [this](MessageIOStream& ios)
					{
						ResourceNetQueries resqueries;
						ResourceNetQuery query;
						while (queries.try_receive(query))
						{
							resqueries.push_back(query);
						}

						if (!resqueries.empty())
						{
							ios << resqueries;
						}

						return !resqueries.empty();
					});
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				{
					std::unique_lock<std::mutex> lock(clientmtx);
					if (client)
					{
						if (!(started = !client->isInInitialState()))
						{
							client = nullptr;
						}
					}
				}
			}
		}).detach();

		launch_named_thread("ResourceClient", [this]
		{
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(30));
				std::unique_lock<std::mutex> lock(clientmtx);
				if (client)
				{
					client->process();
					client->updateFSM();
				}
			}
		}).detach();
	}
}

#endif



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