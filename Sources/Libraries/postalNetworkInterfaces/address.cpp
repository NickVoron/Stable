#include "address.h"

namespace postal
{
	NetworkAddress NetworkAddress::localhost()
	{
		NetworkAddress result;
		result.address = net::address4(127, 0, 0, 1, 0);
		return result;
	}

	NetworkAddress NetworkAddress::localhost(uint16_t port)
	{
		NetworkAddress result;
		result.address = net::address4(127, 0, 0, 1, port);
		return result;
	}

	uint16_t NetworkAddress::port() const
	{
		return address.port();
	}

}