#pragma once
#include "scheme.h"

namespace AppModules
{
	class Autorun : public ModuleBase<Autorun, Loki::MakeTypelist<Scheme>>
	{
	public:
		template<class Params>  void after_init(Params& p) { after_init(); }
		void after_init();
	};
}