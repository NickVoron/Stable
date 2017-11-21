#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <list>

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