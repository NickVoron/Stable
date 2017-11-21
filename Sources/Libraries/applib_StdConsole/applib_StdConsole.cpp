#include "appModule/library.include.h"
#include "applib_Main/library.include.h"
#include "commonAppModules/library.include.h"

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

		template<class Params> void process(Params& params) { ::appmain(params.commandLine); }
	};


	typedef Loki::MakeTypelist<
		CrtMemDebugger,
		Autorun,
		Logs,
		ProcessTerminator,
		AppModules::ComponentModel,
		ResourcesManager,
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
		params.exit = true;
	}
}

int main(int argc, char* argv [])
{	

#define UNIT_TESTS_ENABLED
#ifdef UNIT_TESTS_ENABLED
#define TESTS_ENABLED true
#else 
#define TESTS_ENABLED false
#endif

	try
	{
		appmain<Prologue, 1, TESTS_ENABLED>(&setup, argc, argv);
		return 0;
	}
	catch (const std::exception&)
	{

	}	

	return 1;
}