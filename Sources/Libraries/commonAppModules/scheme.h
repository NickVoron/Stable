#pragma once
#include "scheme/library.include.h"
#include "appModule/library.include.h"

namespace AppModules
{
	class Scheme : public ModuleBase<Scheme>
	{
	public:
		template<class Params>  void before_init(Params& p) {}
	};
}