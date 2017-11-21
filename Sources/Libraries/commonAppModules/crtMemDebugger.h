#pragma once
#include "appModule/library.include.h"

namespace AppModules
{

	class CrtMemDebugger : public ModuleBase<CrtMemDebugger>
	{
	public:
		template<class Params>  void before_init(Params& p) { before_init(); }
		template<class Params>  void process(Params& p) { process(); }
		template<class Params>  void release(Params& p) { release(); }

		void before_init();
		void process();	
		void release();
	};

}