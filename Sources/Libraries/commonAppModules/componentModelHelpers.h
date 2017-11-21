#pragma once

#include "componentModel.h"
#include "screenMouseHelper.h"
									  
namespace AppModules
{	   
	class ComponentModelHelpers : public ModuleBase<ComponentModelHelpers, Loki::MakeTypelist<ComponentModel, ScreenMouseHelper>>
	{
	public:
		template<class Params>  void init(Params& p) { init(); }
		template<class Params>  void process(Params& p) 
		{ 
			process_impl(p.mouseRay);
		}

		void init();
		void process_impl(const intersections::Ray& mouseRay);	
	}; 
}