#include "udp_impl.h"
#include "defaultLogs/library.include.h"


namespace postal
{
	UDPConnection::UDPConnection()
	{
	}

	UDPConnection::UDPConnection(const sockaddr_in& insock)
	{
		address = net::address4(insock);
		sock = std::make_unique<net::Socket>(net::ProtocolType::UDP, false);
	}

	UDPConnection::~UDPConnection()
	{
		close();
	}

	bool UDPConnection::connect(const NetworkAddress& addr)
	{
		LOG_MSG("connect to:" << addr.address.string());
		address = addr.address;
		sock = std::make_unique<net::Socket>(net::ProtocolType::UDP, false);
		return true;
	}

	void UDPConnection::close()
	{
		std::unique_lock<std::mutex> lock(sockmtx);
		sock = nullptr;
	}

	void UDPConnection::sendmsg(const char* msgdata, std::size_t msgsize) const
	{	
		std::unique_lock<std::mutex> lock(sockmtx);
		if (sock && msgsize > 0)
		{
			sock->send(address, msgdata, msgsize);
		}		
	}

	std::size_t UDPConnection::recvmsg(char* msgdata, std::size_t msgsize) const
	{
		std::unique_lock<std::mutex> lock(sockmtx);

		return sock ? sock->receive(msgdata, msgsize) : 0;
	}


	//
	//
	//
	UDPConnections::~UDPConnections()
	{
		close();
	}

	bool UDPConnections::start(uint16_t port)
	{
		serv = std::make_unique<net::Socket>(net::ProtocolType::UDP, port, true);
		return true;
	}

	void UDPConnections::close()
	{		
		serv = nullptr;
	}

	void UDPConnections::stop(std::thread& joinable)
	{
		close();
		joinable.join();
	}

	UDPConnection* UDPConnections::incoming()
	{
		std::unique_lock<std::mutex> lock(sockmtx);
		
		UDPConnection* res = nullptr;
		struct sockaddr_in dest;
		int data;
		while (true)
		{
			if (serv->receive((char*) &data, sizeof(data), dest))
			{
				return new UDPConnection(dest);
			}
		}		
		
		return res;
	}
}