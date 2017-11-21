#pragma once

#include <thread>
#include <mutex>

#include "address.h"

#include "stream/library.include.h"
#include "atomicNetCore/library.include.h"
#include "libutp/library.include.h"

namespace postal
{
	struct UTPConnection
	{
		~UTPConnection();

		bool connect(const NetworkAddress& addr);
		void close();

		NetworkAddress address() const;

		std::size_t sendmsg(const char* msgdata, std::size_t msgsize) const;
		std::size_t recvmsg(char* msgdata, std::size_t msgsize) const;

		std::string name = "unknown-side";
		
		mutable std::mutex sockmtx;
		utp_socket* sock = nullptr;
	};

	struct UTPConnections
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

		~UTPConnections();
		bool start(unsigned short port);
		void stop(std::thread& joinable);
		void close();
			
		UTPConnection* incoming();

		mutable std::mutex sockmtx;
	};
}