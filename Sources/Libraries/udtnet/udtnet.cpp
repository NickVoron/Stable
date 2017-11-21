#include "udtnet.h"


#include "defaultLogs/library.include.h"

#include <iostream>
#include <thread>


// std::ostream& operator<<(std::ostream& os, const UDT::ERRORINFO& e)
// {
// 	return os << ((UDT::ERRORINFO&)e).getErrorMessage();
// }
// 
namespace udtnet
{

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



	void init()
	{
		UDT::startup();
	}

	void release()
	{
		UDT::cleanup();
	}

	void Connections::dispatch()
	{
		IStream is;
		
		while (!exitDispatcher)
		{
			for (auto& connect : *this)
			{
				is.sizeref() = connect->recvmsg(is.bufptr(), is.capacity());
				if (is.size() > 0)
				{
					connect->dispatch(is);
					is.resetReadCursor();
				}				
			}
		}
	}


	Connections::~Connections()
	{
		stop();
	}

	void Connections::stop()
	{
		try
		{
			if (serv)
			{
				UDT::close(serv);
				serv = 0;
			}			

			exitConnection = true;
			exitDispatcher = true;

			if (connections.joinable()) connections.join();
			if (dispatcherThread.joinable()) dispatcherThread.join();

			clear();
		}
		catch (const std::exception& e)
		{
			LOG_ERROR(e.what());
		}
		
	}

	bool Connections::server(unsigned short port, std::function<DataDispatcher*()> factory)
	{	
		// Создадим сокет для связи клиентов с сервером
		addrinfo hints;
		addrinfo* res;
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_flags = AI_PASSIVE;
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_DGRAM;
		if (0 != getaddrinfo(NULL, str::stringize(port).c_str(), &hints, &res)) 
		{ 
			LOG_ERROR("illegal port number or port is busy, or UDT isn't initialized: " << port)
			return false; 
		}
		serv = UDT::socket(res->ai_family, res->ai_socktype, res->ai_protocol);

		if (UDT::ERROR == UDT::bind(serv, res->ai_addr, res->ai_addrlen)) 
		{ 
			//LOG_MSG("bind: " << UDT::getlasterror());
			return false; 
		}

		freeaddrinfo(res);

		exitConnection = false;
		exitDispatcher = false;

		dispatcherThread = std::thread([this]
		{ 
			dispatch();
			LOG_MSG("SERVER EXIT DISPATCHER THREAD")
		});	   

		connections = std::thread([this, factory]
		{
			LOG_MSG("server is ready");

			if (UDT::ERROR == UDT::listen(serv, 10))
			{
				//LOG_ERROR("listen: " << UDT::getlasterror());
				return false;
			}

			// Ждём подключения клиентов
			while (!exitConnection)
			{
 				sockaddr_storage clientaddr;
 				int addrlen = sizeof(clientaddr);
 				UDTSOCKET recver = UDT::accept(serv, (sockaddr*) &clientaddr, &addrlen);
 				if (UDT::INVALID_SOCK == recver)
 				{
					if (!exitConnection)
					{
						//LOG_ERROR("accept: " << UDT::getlasterror());
					} 					
					continue;;
				}
				else
				{
					char clienthost[NI_MAXHOST];
					char clientservice[NI_MAXSERV];
					getnameinfo((sockaddr *) &clientaddr, addrlen, clienthost, sizeof(clienthost), clientservice, sizeof(clientservice), NI_NUMERICHOST | NI_NUMERICSERV);
					LOG_MSG("new connection: " << clienthost << ":" << clientservice << " " << "connections count: " << size() + 1);

					addConnection(recver, factory());
				}
			}

			if (serv)
			{
				UDT::close(serv);
				serv = 0;
			}			

			LOG_MSG("SERVER EXIT CONNECTIONS THREAD")
			return true;
		});

		return true;
	}

	void Connections::addConnection(UDTSOCKET connect, DataDispatcher* dispatcher)
	{	  
		std::unique_lock<std::mutex> lock(lockmtx);

		emplace_back(new Connection(connect, dispatcher));
		back()->dispatcherStart();
	}

	void Connections::internalMessage(udtnet::IOStream& is)
	{
		std::unique_lock<std::mutex> lock(lockmtx);

		for (auto& connect : *this)
		{
			connect->internalMessage(is);
			is.reset();
		}
	}


	//
	//
	//
	Connection::~Connection()
	{
	}

	void Connection::dispatcherStart()
	{
		ENFORCE(dispatcher);
		dispatcher->start(*this);
	}

	void Connection::dispatch()
	{
		IStream is;

		while (!exitDispatcher)
		{
			is.sizeref() = recvmsg(is.bufptr(), is.capacity());
			if (is.size() > 0)
			{
				ENFORCE(is.left() == is.size());
				dispatch(is);
				is.resetReadCursor();
			}			
		}
	}

	void Connection::dispatch(IStream& is)
	{				
		if (is.size() > 0 && dispatcher)
		{
			dispatcher->recvmsg(is);
		}
	}

	bool Connection::connect(const Address& addr)
	{
		// СОКЕТ для связи с сервером
		struct addrinfo hints, *local, *peer;
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_flags = AI_PASSIVE;
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_DGRAM;
		if (0 != getaddrinfo(NULL, addr.port_str().c_str(), &hints, &local))
		{
			LOG_MSG("incorrect network address");
			return false;
		}

		sock = UDT::socket(local->ai_family, local->ai_socktype, local->ai_protocol);
		freeaddrinfo(local);

		if (0 != getaddrinfo(addr.addr_str().c_str(), addr.port_str().c_str(), &hints, &peer))
		{
			LOG_ERROR("incorrect server/peer address");
			close();
			return false;
		}

		if (UDT::ERROR == UDT::connect(sock, peer->ai_addr, peer->ai_addrlen))
		{
			//LOG_ERROR("connect: " << UDT::getlasterror());
			close();
			return false;
		}
		freeaddrinfo(peer);

		std::thread ct0([this]
		{
			//LOG_MSG("START CLIENT");
			dispatch();
			//LOG_MSG("STOP CLIENT");
		});
		ct0.detach();

		//LOG_MSG("connected to server");

		exitDispatcher = false;
		return true;
	}

	void Connection::close()
	{
		exitDispatcher = true;
		if (sock)
		{
			//UDT::close(sock);
			sock = 0;
		}		
	}

	void Connection::sendmsg(const mem::mem_desc& memblock) const
	{
		sendmsg((const char*)memblock.data, memblock.len);
	}

	void Connection::sendmsg(const char* msgdata, int msgsize) const
	{
		if (msgsize > 0)
		{
			//LOG_MSG("report : " << msgsize);
			UDT::sendmsg(sock, msgdata, msgsize, -1, true);
		}
	}

	int Connection::recvmsg(char* msgdata, int msgsize) const
	{
		int rcv_size, rs;
		int var_size = sizeof(int);
		UDT::getsockopt(sock, 0, UDT_RCVDATA, &rcv_size, &var_size);
		if (rcv_size == 0)
			return 0;
		if (UDT::ERROR == (rs = UDT::recvmsg(sock, msgdata, msgsize)))
		{
			//LOG_ERROR("recv:" << UDT::getlasterror());
			return 0;
		}
		return rs;
	}

	void Connection::internalMessage(udtnet::IOStream& is)
	{
		ENFORCE(dispatcher);
		dispatcher->internalMessage(is);
	}

}