#pragma once

#include "commandProcessor/library.include.h"

namespace AppModules
{

class CommandProcessor : public ModuleBase<CommandProcessor>
{
public:
	template<class Params>  void init(Params& p)		{ CmdProc::CommandsManager::init();		}
	template<class Params>  void process(Params& p)	{ CmdProc::CommandsManager::process();	}
	template<class Params>  void release(Params& p)	{ CmdProc::CommandsManager::release();	}
};

}