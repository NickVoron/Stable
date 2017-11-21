#include "lanDiscover.h"
#include "stuff/library.include.h"

namespace net
{
	lan_discover_udp_node::lan_discover_udp_node() : sock(ProtocolType::UDP, 55566, false)
	{
		
	}

	void lan_discover_udp_node::request(uint16_t port) const
	{
		static int i = 0;
		host_info host;
		auto lanaddr = host.getLAN();

        i++;
//		LOG_MSG(i << " lan_discover_udp_node: " << lanaddr.first << " : " << lanaddr.second.addr_str());
		if (lanaddr.first)
		{
			sock.send(lanaddr.second.broadcast(port), Request());
		}					
	}

	std::vector<net::address4> lan_discover_udp_node::result() const
	{
		std::vector<net::address4> result;
		net::address4 addr;
		Answer answ;
		while (sock.receive((char*)&answ, sizeof(answ), addr))
		{
			if (std::find_if(host.addresses.begin(), host.addresses.end(), [ip = addr.ip()](auto& addr){ return addr.ip() == ip; }) == host.addresses.end())
			{
//				LOG_EXPRESSION(addr);
				result.emplace_back(addr);
			}			
		}

		return result;
	}

	void lan_discover_udp_node::answer() const
	{
		net::address4 addr;
		Request req, ethalon;
		while (sock.receive((char*) &req, sizeof(req), addr))
		{
			if (memcmp(&req, &ethalon, sizeof(req)) == 0)
			{
				LOG_MSG("answer to: " << addr.addr_str());
				sock.send(addr, Answer());
				return;
			}			
		}
	}

	//
	//
	//
	void udp_packets_logger::listen(uint16_t port)
	{
		sock = std::make_unique<net::Socket>(ProtocolType::UDP, port, false);
	}

	void udp_packets_logger::read()
	{
		if (sock)
		{
			net::address4 addr;
			char data[65535];
			while (auto rcv = sock->receive((char*) &data[0], sizeof(data), addr))
			{
				LOG_MSG(str::spaced("udp_packets_logger::read() : on local port:", sock->port(), rcv, "bytes from:", addr).str());
			}
		}		
	}

	//
	//
	//
	udp_packets_sender::udp_packets_sender() : sock(ProtocolType::UDP, false)
	{

	}

	void udp_packets_sender::send(const net::address4& addr, uint16_t size)
	{
		char buffer[65536];
		sock.send(addr, &buffer[0], size);
	}
}