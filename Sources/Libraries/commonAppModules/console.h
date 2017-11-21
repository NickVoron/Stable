#pragma once
#include "appModule/library.include.h"
#include "console/library.include.h"
#include "logs.h"
#include "window.h"

namespace AppModules
{
	class Console : public ModuleBase<Console, Loki::NullType, Loki::MakeTypelist<Logs, Window>>
	{
	public:
		template<class Params>  void before_init(Params& p)	{ before_init_impl(); }
		template<class Params>  void release(Params& p)		{ release_impl(); }

	private:
		void before_init_impl();
		void release_impl();
	};
}