#include "appModule/library.include.h"
#include "applib_Main/library.include.h"
#include "commonAppModules/library.include.h"
#include "common/thread_utils.h"

extern void init();
extern void release();
extern void appmain(const std::vector<std::string>& commandLine);

namespace
{
	using namespace AppModules;

	struct Application : public ModuleBase<Application>
	{
		typedef typename Loki::NullType Prologue;
		typedef typename Loki::NullType Epilogue;

		struct Params
		{
			std::vector<std::string> commandLine;
		};

		template<class Params> void init(Params& params) { ::init(); }
		template<class Params> void process(Params& params) { ::appmain(params.commandLine); }
	};


	typedef Loki::MakeTypelist<
		Locale,
		DllDir,
		WorkDir,
		CrtMemDebugger,
		Autorun,
		Logs,
		QtLog,
		LogsUpdater,
		UTPNet,
		ProcessTerminator,
		AppModules::ComponentModel,
		ResourcesManager,
		ResourceWatcher,
		Application
	> Prologue;

	struct Params
	{
		template<class T>
		Params(T& params) : exit(params.exit), commandLine(params.commandLine) {}
		bool& exit;
		std::vector<std::string>& commandLine;
	};

	void setup(Params& params)
	{
		params.exit = false;
	}
}

namespace common
{
	int main(int argc, char* argv [])
	{
		return appmain<Prologue, 0, true>(&setup, argc, argv);
	}	
}


#if defined(WIN32)
int main(int argc, char* argv [])
{
	return common::main(argc, argv);
}
#endif

#if defined(__ANDROID__)
#include <jni.h>
namespace applib
{
	void main(AAssetManager* manager)
	{
		stream::manager = manager;

		launch_named_thread("applib::main", [] 
		{
			char *argv [] = { "android", 0 };
			common::main(1, argv);
		}).detach();
	}
}
#endif

#if defined(__APPLE__)
namespace applib
{
	void main()
	{
		launch_named_thread("applib::main", [] 
		{
			char *argv [] = { "iOS", 0 };
			common::main(1, argv);
		}).detach();
	}
}
#endif
