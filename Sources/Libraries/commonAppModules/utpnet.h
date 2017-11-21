#pragma once
#include "appModule/library.include.h"

namespace AppModules
{	   
	class UTPNet : public ModuleBase<UTPNet>
	{
	public:
		template<class Params>  void init(Params& p) { init(); }
		template<class Params>  void process(Params& p) { process(); }
		template<class Params>  void release(Params& p) { release(); }

		void init();
		void process();
		void release();
	}; 
}