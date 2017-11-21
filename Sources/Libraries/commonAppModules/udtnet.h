#pragma once

#include "udtnet/library.include.h"

namespace AppModules
{	   
	class UDTNet : public ModuleBase<UDTNet>
	{
	public:
		template<class Params>  void init(Params& p) { init(); }
		template<class Params>  void release(Params& p) { release(); }

		void init();
		void release();
	}; 
}