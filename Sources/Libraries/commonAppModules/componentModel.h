#pragma once

#include "autorun.h"
#include "utpnet.h"
#include "timer.h"

									  
namespace AppModules
{	   
	class ComponentModel : public ModuleBase<ComponentModel, Loki::MakeTypelist<Autorun, Timer, UTPNet>>
	{
	public:
		template<class Params>  void init(Params& p) { init(); }
		template<class Params>  void process(Params& p) { process(p.dt); }

		void init();
		void process(float dt);	
	}; 
}