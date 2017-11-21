#pragma once
#include "appModule/library.include.h"

#include "streamer.h"
#include "settings.h"
#include "freeimage.h"
#include "logs.h"

namespace AppModules
{
	class ResourcesManager : public ModuleBase<ResourcesManager, Loki::MakeTypelist<FreeImage, Settings, Logs, Streamer>>
	{
	public:
		ResourcesManager();

		template<class Params>  void before_init(Params& p)	{ before_init_impl(p.commandLine); }
		template<class Params>  void init(Params& p)		{ init_impl(); }
		template<class Params>  void after_init(Params& p)	{ after_init_impl(p.exit); }
		template<class Params>  void release(Params& p)		{ release_impl(); }

	private:
		void before_init_impl(const std::vector<std::string>& commandLine);
		void init_impl();
		void after_init_impl(bool& exit);
		void release_impl();
	};

	class ResourceWatcher : public ModuleBase<ResourceWatcher, Loki::MakeTypelist<ResourcesManager>>
	{
	public:
		template<class Params>  void before_init(Params& p)		{ before_init(); }
		template<class Params>  void process(Params& p) { process(); }
		template<class Params>  void release(Params& p)	{ release(); }

		void before_init();
		void process();
		void release();
	};

}