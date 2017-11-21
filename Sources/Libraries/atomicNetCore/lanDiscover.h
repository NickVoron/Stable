#pragma once

#include "os.h"

#include <memory>
#include <vector>

#include "address.h"


namespace net
{
	struct lan_discover_udp_node
	{
		struct Request	{ uint8_t data[4] = {'A', 'C', 'K', 'K'}; };
		struct Answer	{ uint8_t data[4] = { 'A', 'N', 'S', 'W' }; };

		lan_discover_udp_node();

		void request(uint16_t port) const;
		std::vector<net::address4> result() const;
		void answer() const;

		net::Socket sock;
		host_info host;
	};

	struct udp_packets_logger
	{
		void listen(uint16_t port);
		void read();

		std::unique_ptr<net::Socket> sock;
	};

	struct udp_packets_sender
	{
		udp_packets_sender();
		void send(const net::address4& addr, uint16_t size);
		net::Socket sock;
	};
}