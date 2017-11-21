#pragma once

#include "appModule/library.include.h"
#include "stuff/library.include.h"

namespace AppModules
{
	class Timer : public ModuleBase<Timer>
	{
	public:

		struct Params
		{
			Params();
			float fps;
			float dt;
			float timeFactor;
		};

		template<class Params>  void after_init(Params& p)	{ after_init_impl(p); }
		template<class Params>  void before_process(Params& p)	{ update(p); }
		

		void update(Params& params);
		void after_init_impl(Params& params);

		float fps(bool average);
		float dt(bool average);
	private:
		Base::Timer timer;
		float lastTime;
		float lastFPS;
	};
}