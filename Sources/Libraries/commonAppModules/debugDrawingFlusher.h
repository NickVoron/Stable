#pragma once
#include "appModule/library.include.h"

namespace AppModules
{

struct DebugDrawingFlusher : public ModuleBase<DebugDrawingFlusher>
{
	template<class Params>  void process(Params& p)	{ process(); }
	void process();
};

}