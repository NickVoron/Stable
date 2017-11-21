#pragma once

#include "appModule/library.include.h"

namespace AppModules
{
	class JNI : public ModuleBase<JNI>
	{
	public:
		template<class Params> void before_process(Params& p)	{ before_process(); }
		template<class Params> void after_process(Params& p)	{ after_process(); }
		
		void before_process();
		void after_process();
	};
}