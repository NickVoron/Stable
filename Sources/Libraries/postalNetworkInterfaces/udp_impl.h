#pragma once

#include <thread>
#include <mutex>

#include "address.h"

#include "stream/library.include.h"
#include "atomicNetCore/library.include.h"
#include "udt/library.include.h"

namespace postal
{
	struct UDPConnection
	{
		UDPConnection();
		UDPConnection(const sockaddr_in& insock);

		~UDPConnection();

		bool connect(const NetworkAddress& addr);
		void close();

		void sendmsg(const char* msgdata, std::size_t msgsize) const;
		std::size_t recvmsg(char* msgdata, std::size_t msgsize) const;

		std::string name = "unknown-side";

		mutable std::mutex sockmtx;
		std::unique_ptr<net::Socket> sock;
		net::address4 address;
	};

	struct UDPConnections
	{
		template<class Function>
		bool accept(Function&& function)
		{
			if (auto res = incoming())
			{
				function(res);
				return true;
			}
			return false;
		}

		~UDPConnections();
		bool start(uint16_t port);
		void stop(std::thread& joinable);
		void close();
			
		UDPConnection* incoming();

		mutable std::mutex sockmtx;
		std::unique_ptr<net::Socket> serv;
	};
}