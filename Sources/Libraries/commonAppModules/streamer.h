#pragma once
#include "appModule/library.include.h"
#include "streaming/library.include.h"

namespace AppModules
{

class Streamer : public ModuleBase<Streamer>
{
public:
	template<class Params>  void before_init(Params& p) { Streaming::Streamer::init(); }
	template<class Params>  void before_release(Params& p)	{ Streaming::Streamer::release(); }
};


}