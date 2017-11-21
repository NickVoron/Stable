#pragma once

#include "appModule/library.include.h"

namespace AppModules
{

class ModuleName : public Module
{
public:

	template<class Params> void init(Params& p)		{ }
	template<class Params> void process(Params& p)	{ }
	template<class Params> void release(Params& p)	{ }
};

}