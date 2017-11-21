#include "utpnet.h"

#include "libutp/library.include.h"

namespace postal
{
	std::unique_ptr<utp::core> kernel;
}

namespace AppModules
{
	void UTPNet::init()
	{
		net::init();
	}
	
	void UTPNet::process()
	{
		if (postal::kernel)
		{
			postal::kernel->update_udp();
		}		
	}

	void UTPNet::release()
	{
		net::release();
	}
}
