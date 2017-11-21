#include "crtMemDebugger.h"

namespace AppModules
{

#pragma init_seg(lib)

	struct DebugSetter 
	{

		DebugSetter()
		{
#ifdef WIN32
			//_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);
			_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
			// 		_CrtMemCheckpoint(&_ms);
			//_CrtSetBreakAlloc(45505);
#endif
		}
	};

	DebugSetter setter;


	void CrtMemDebugger::before_init()
	{
		
	}

	void CrtMemDebugger::process()
	{
#ifdef WIN32
		_CrtCheckMemory();
#endif
	}

	void CrtMemDebugger::release()
	{
#ifdef WIN32
		_CrtCheckMemory();
#endif
	}

}
