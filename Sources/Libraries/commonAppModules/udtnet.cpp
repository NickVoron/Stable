#include "udtnet.h"

namespace AppModules
{
	void UDTNet::init()
	{
		UDT::startup();
	}
	
	void UDTNet::release()
	{
		UDT::cleanup();
	}

}
