#pragma once

#include "defaultLogs/library.include.h"
#include "workDir.h"
#include "locale.h"

namespace AppModules
{

struct Logs : public ModuleBase<Logs, Loki::MakeTypelist<WorkDir>, Loki::MakeTypelist<Locale>>
{
	template<class Params>  void after_process(Params& p) { LOG_REPORT_CLEAR; };
};

}