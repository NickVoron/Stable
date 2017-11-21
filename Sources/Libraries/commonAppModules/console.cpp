#include "console.h"

namespace AppModules
{
	void Console::before_init_impl() 
	{ 
		Base::Console::create(); 
	}

	void Console::release_impl() 
	{ 
		Base::Console::destroy(); 
	}
}