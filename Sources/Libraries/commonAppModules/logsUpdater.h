#pragma once

#include "logs.h"
#include "timer.h"

namespace AppModules
{
	struct LogsUpdater : public ModuleBase<LogsUpdater, Loki::MakeTypelist<Logs>>
	{
		template<class Params>  void process(Params& p)	{  logs::update(0.03f); 	};
	};
}