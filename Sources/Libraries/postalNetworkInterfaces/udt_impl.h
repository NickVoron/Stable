#pragma once

#include <thread>
#include <mutex>

#include "address.h"

#include "stream/library.include.h"
#include "atomicNetCore/library.include.h"
#include "udt/library.include.h"

namespace postal
{
	struct UDTConnection
	{
		~UDTConnection();

		bool connect(const NetworkAddress& addr);
		void close();

		void sendmsg(const char* msgdata, std::size_t msgsize) const;
		std::size_t recvmsg(char* msgdata, std::size_t msgsize) const;

		std::string name = "unknown-side";

		mutable std::mutex sockmtx;
		UDTSOCKET sock = UDT::INVALID_SOCK;
	};

	struct UDTConnections
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

		~UDTConnections();
		bool start(unsigned short port);
		void stop(std::thread& joinable);
		void close();
			
		UDTConnection* incoming();

		mutable std::mutex sockmtx;
		UDTSOCKET serv = UDT::INVALID_SOCK;
	};
}

std::ostream& operator<<(std::ostream& os, const UDT::ERRORINFO& e);