#pragma once

#include "appModule/library.include.h"

namespace AppModules
{
	class OpenAL : public ModuleBase<OpenAL>
	{
	public:
		template<class Params> void init(Params& p)		{ init(); }
		template<class Params> void process(Params& p)	{ process(p.dt, p.currentCalculatedCamera.state); }
		template<class Params> void release(Params& p)	{ release(); }

		void init();
		void process(float dt, const State& camera);
		void release();
	};

}