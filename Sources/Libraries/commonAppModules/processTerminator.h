#pragma once
#include <thread>
#include "appModule/library.include.h"

namespace AppModules
{
	class ProcessTerminator : public ModuleBase<ProcessTerminator>
	{
	public:
		template<class Params>  void init(const Params& p)		{ init(); }
		template<class Params>  void release(const Params& p) { release(); }

		void init();
		void release();

		std::thread thread;

		bool exitthread = false;
	};
}