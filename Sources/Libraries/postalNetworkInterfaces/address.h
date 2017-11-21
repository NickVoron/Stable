#pragma once

#include <stdint.h>
#include "atomicNetCore/library.include.h"

namespace postal
{
	struct NetworkAddress
	{
		static NetworkAddress localhost();
		static NetworkAddress localhost(uint16_t port);

		uint16_t port() const;

		net::address4 address;
	};
}




