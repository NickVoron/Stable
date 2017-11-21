#pragma once

#include "settingsRegistry/library.include.h"

#include "workDir.h"

namespace AppModules
{

	class Settings : public ModuleBase<Settings, Loki::MakeTypelist<WorkDir>>
	{
	public:
		template<class Params>  void before_init(const Params& p) { before_init_impl(); }
		template<class Params>  void release(Params& p) { release_impl(); }

		void before_init_impl() 
		{ 
			SettingsRegistry::create();
			SettingsRegistry::load(); 
		}

		void release_impl() 
		{ 
			SettingsRegistry::save(); 
			SettingsRegistry::destroy();
		}
	};

}