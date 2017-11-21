#include "udt_impl.h"
#include "defaultLogs/library.include.h"

#ifndef WIN32
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#include <wspiapi.h>
#endif

#pragma comment (lib, "ws2_32.lib")

namespace postal
{
	UDTConnection::~UDTConnection()
	{
		close();
	}

	bool UDTConnection::connect(const NetworkAddress& addr)
	{
		LOG_MSG("connect to:" << addr.address.string());

		struct addrinfo hints, *local, *peer;
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_flags = AI_PASSIVE;
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_DGRAM;
		std::string port = addr.address.port_str();
		if (0 != getaddrinfo(NULL, port.c_str(), &hints, &local))
		{
			LOG_MSG("incorrect network address");
			return false;
		}

		{
			std::unique_lock<std::mutex> lock(sockmtx);
			sock = UDT::socket(local->ai_family, local->ai_socktype, local->ai_protocol);
		}
		
		freeaddrinfo(local);

		if (0 != getaddrinfo(addr.address.addr_str().c_str(), port.c_str(), &hints, &peer))
		{
			LOG_ERROR("incorrect server/peer address");
			close();
			return false;
		}
		
		bool connected = false;
		{
			std::unique_lock<std::mutex> lock(sockmtx);
			connected = (UDT::ERROR != UDT::connect(sock, peer->ai_addr, peer->ai_addrlen));
		}		
		
		if (!connected)
		{
			LOG_ERROR("connect: " << UDT::getlasterror());
			close();
			return false;
		}
		freeaddrinfo(peer);

		return true;
	}

	void UDTConnection::close()
	{
		std::unique_lock<std::mutex> lock(sockmtx);
		if (sock != UDT::INVALID_SOCK)
		{
			UDT::close(sock);
			sock = UDT::INVALID_SOCK;
		}
	}

	void UDTConnection::sendmsg(const char* msgdata, std::size_t msgsize) const
	{	
		std::unique_lock<std::mutex> lock(sockmtx);
		if ((sock != UDT::INVALID_SOCK) && (msgsize > 0) )
		{
			UDT::sendmsg(sock, msgdata, msgsize, -1, true);
		}		
	}

	std::size_t UDTConnection::recvmsg(char* msgdata, std::size_t msgsize) const
	{
		std::unique_lock<std::mutex> lock(sockmtx);

		int rs = 0;
		if (sock != UDT::INVALID_SOCK)
		{
			int rcv_size = 0;
			int var_size = sizeof(int);

			UDT::getsockopt(sock, 0, UDT_RCVDATA, &rcv_size, &var_size);
			if (rcv_size != 0)
			{
				if (UDT::ERROR == (rs = UDT::recvmsg(sock, msgdata, msgsize)))
				{
					LOG_ERROR("recv:" << UDT::getlasterror());
				}
			}
		}		
		
		return rs;
	}


	//
	//
	//
	UDTConnections::~UDTConnections()
	{
		close();
		ENFORCE(serv == UDT::INVALID_SOCK);		
	}

	bool UDTConnections::start(unsigned short port)
	{
		addrinfo hints;
		addrinfo* res;
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_flags = AI_PASSIVE;
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_DGRAM;
		if (0 != getaddrinfo(NULL, str::stringize(port).c_str(), &hints, &res))
		{
			LOG_ERROR("illegal port number or port is busy, or UDT isn't initialized: " << port);
			return false;
		}

		{
			std::unique_lock<std::mutex> lock(sockmtx);
			serv = UDT::socket(res->ai_family, res->ai_socktype, res->ai_protocol);

			if (UDT::ERROR == UDT::bind(serv, res->ai_addr, res->ai_addrlen))
			{
				LOG_ERROR("bind: " << UDT::getlasterror());
				return false;
			}

			if (UDT::ERROR == UDT::listen(serv, 10))
			{
				LOG_ERROR("listen: " << UDT::getlasterror());
				return false;
			}

			freeaddrinfo(res);
		}

		return true;
	}

	void UDTConnections::close()
	{		
		if (serv != UDT::INVALID_SOCK)
		{
			UDT::close(serv);
			std::unique_lock<std::mutex> lock(sockmtx);
			serv = UDT::INVALID_SOCK;
		}
	}

	void UDTConnections::stop(std::thread& joinable)
	{
		close();
		joinable.join();
	}

	UDTConnection* UDTConnections::incoming()
	{
		std::unique_lock<std::mutex> lock(sockmtx);
		
		UDTConnection* res = nullptr;

		if (serv != UDT::INVALID_SOCK)
		{
			sockaddr_storage clientaddr;
			int addrlen = sizeof(clientaddr);
			UDTSOCKET recver = UDT::accept(serv, (sockaddr*) &clientaddr, &addrlen);
			if (UDT::INVALID_SOCK != recver)
			{
				char clienthost[NI_MAXHOST];
				char clientservice[NI_MAXSERV];
				getnameinfo((sockaddr *) &clientaddr, addrlen, clienthost, sizeof(clienthost), clientservice, sizeof(clientservice), NI_NUMERICHOST | NI_NUMERICSERV);
				LOG_MSG("new connection: " << clienthost << ":" << clientservice);

				res = new UDTConnection();
				res->sock = recver;
			}
			else
			{
				LOG_ERROR("accept: " << UDT::getlasterror());
			}
		}

		return res;
	}
}

std::ostream& operator<<(std::ostream& os, const UDT::ERRORINFO& e)
{
	return os << ((UDT::ERRORINFO&)e).getErrorMessage();
}