// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <chrono>
#include <array>

#include "common/thread_utils.h"

namespace postal
{
	struct ActivityTracker
	{
		ActivityTracker()
		{
			timeToLive = 2000.0f;
			startTime = std::chrono::high_resolution_clock::now();
			refresh();
		}

		void refresh() const
		{
			lastRecvTime = std::chrono::high_resolution_clock::now();

		}

		float aliveFactor() const
		{
			float dtt = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - lastRecvTime).count());
			return 1.0f - std::min(std::max((dtt / timeToLive), 0.0f), 1.0f);
		}

		float timeToLive;
		mutable std::chrono::high_resolution_clock::time_point lastRecvTime;
		std::chrono::high_resolution_clock::time_point startTime;
	};

	struct DataTracker
	{
		uint64_t totalDataSize = 0;
		uint64_t totalPacketsCount = 0;

		void add(std::size_t dataSize)
		{
			totalDataSize += totalPacketsCount;
			++totalPacketsCount;
		}
	};

	template<class ConnectInterface, class MessageDispatcher, class Address, class MessageIStream, class MessageIOStreamT>
	class Connection : public Base::NonCopyable
	{
	public:
		typedef Connection ConnectionType;
		typedef MessageIOStreamT MessageIOStream;

		Connection(ConnectInterface* connect) : connection(connect)
		{ 
			defaults();
			dispatcher.start();
		}

		Connection()
		{
			defaults();
		}
		
		void defaults()
		{
			dispatcher.fsm.connection = (decltype(dispatcher.fsm.connection))this;
			activityTracker = ActivityTracker();
		}

		bool connect(const Address& addr)
		{
			connection = std::make_unique<ConnectInterface>();
			return connection->connect(addr);
		}

		void stop() {}

		void updateFSM(MessageIStream& is)
		{
			is.sizeref() = recvmsg(is.bufptr(), is.capacity());
			if (is.size() > 0)
			{
				ENFORCE(is.left() == is.size());
				dispatch(is);
				is.resetReadCursor();
			}
		}

		void runFSM()
		{
			this->exitDispatcher = false;
			MessageIStream is;
			while (!this->exitDispatcher)
			{
				update(is);
			}
		}


		void dispatch(MessageIStream& is)
		{
			if (is.size() > 0)
			{
				activityTracker.refresh();
				dataTrackers[1].add(is.size());

				dispatcher.recvmsg(is);
			}
		}

		std::size_t sendmsg(const mem::mem_desc& data) const
		{ 
			return sendmsg((const char*) data.data, data.len);
		}

		std::size_t sendmsg(const char* msgdata, std::size_t msgsize) const 
		{	
			
			auto sended = connection->sendmsg(msgdata, msgsize);
			
			dataTrackers[0].add(sended);
			return sended;
		}

		template<class T>
		std::size_t sendmsg(const T& data) const
		{
			return sendmsg((const char*) &data, sizeof(data)); 
		}

		std::size_t recvmsg(mem::mem_desc& data) const	{	return recvmsg(data.data, data.len);	}
		
		std::size_t recvmsg(char* msgdata, std::size_t msgsize) const	
		{				
			std::size_t rs = connection->recvmsg(msgdata, msgsize);

			return rs;
		}

		Address address() const { return connection->address(); }

		template<class T>
		std::size_t recvmsg(T& data) const { return recvmsg((char*) &data, sizeof(data)); }

		bool isInInitialState() const 
		{
			return dispatcher.fsm.isInInitialState();
		}		

		void internalMessage(MessageIOStream& is) { 	dispatcher.internalMessage(is); }

		void debugName(const std::string& name) { connection->name = name; }
		std::string debugName() const { return connection->name; }
		
		ActivityTracker activityTracker;
		mutable std::array<DataTracker, 2> dataTrackers;

		std::unique_ptr<ConnectInterface> connection;
		MessageDispatcher dispatcher;
		
	};

	template<class Connection>
	class ConnectionsContainer : std::list<std::unique_ptr<Connection>>
	{
	public:
		~ConnectionsContainer()
		{
			std::unique_lock<std::mutex> lock(mtx);
			ConnectionsContainer::clear();
		}

		void add(Connection* connection)
		{
			std::unique_lock<std::mutex> lock(mtx);
			ConnectionsContainer::emplace_back(connection);
		}

		void remove(Connection* connection)
		{
			std::unique_lock<std::mutex> lock(mtx);
			std::list<std::unique_ptr<Connection>>::remove(connection);
		}

		template<class Function>
		void update(Function&& function)
		{
			std::unique_lock<std::mutex> lock(mtx);



			for (auto& connect : *this)
			{
				function(connect);
			}
		}

		auto size() const { return std::list<std::unique_ptr<Connection>>::size(); }

	private:
		std::mutex mtx;
	};

	template<class ConnectionInterface, class ConnectionsInterface, class TransitionsList, class Address, class MessageIStream, class MessageIOStream, class EVENT_TYPE>
	class Connections
	{
	public:
		struct Connection;
		struct DataDispatcher;

		struct StateBase : public postal::BaseStateT<MessageIOStream, MessageIStream, Connection, DataDispatcher, EVENT_TYPE> {};
		struct Start : public postal::StartState<MessageIOStream, MessageIStream, Connection, StateBase> {};
		struct DataDispatcher : public postal::DataDispatcherT<MessageIOStream, MessageIStream, Connection, StateBase, TransitionsList> {};
		struct Connection : public postal::Connection<ConnectionInterface, DataDispatcher, Address, MessageIStream, MessageIOStream> 
		{
			Connection() {}
			Connection(ConnectionInterface* connection) : Connection::ConnectionType(connection) {}
		};

		Connections() :exitConnections(true) {}

		void updateFSM(MessageIStream& is)
		{
			connections.update([&is](auto& connect)
			{
				is.sizeref() = connect->recvmsg(is.bufptr(), is.capacity());
				if (is.size() > 0)
				{
					connect->dispatch(is);
					is.resetReadCursor();
				}
			});
		}

		void runFSM()
		{
			this->exitDispatcher = false;
			MessageIStream is;
			while (!this->exitDispatcher)
			{
				updateFSM(is);
			}
		}
		
		bool server(uint16_t port)
		{
			if (connectionsInterface.start(port))
			{
				exitConnections = false;
				connectionsThread = launch_named_thread("PostalServer", [this]
				{
					LOG_MSG("postal connections server started");
					bool result = true;
					while (!exitConnections && result)
					{
						result &= connectionsInterface.accept([this](auto clientConnection)
						{
							connections.add(new Connection(clientConnection));
						});
					}
					LOG_MSG("postal connections server finished");
					return true;
				});

				return true;
			}

			return false;
		}
				
		void stop()
		{
			if (connectionsThread.joinable())
			{
				exitConnections = true;
				connectionsInterface.stop(connectionsThread);
			}
		}

		void internalMessage(MessageIOStream& is)
		{
			connections.update([&is](auto& connect)
			{
				connect->internalMessage(is);
				is.reset();
			});
		}

		std::size_t connectionsCount() const { return connections.size(); }

	private:
		std::thread connectionsThread;
		std::atomic_bool exitConnections;

		ConnectionsInterface connectionsInterface;
		ConnectionsContainer<Connection> connections;
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