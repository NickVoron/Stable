#include "processTerminator.h"

#ifdef WIN32
#include "imgui/library.include.h"
#include "imguiModules/library.include.h"
#endif

#include <thread>

#include "common/thread_utils.h"

namespace AppModules
{	
#ifdef WIN32
	struct ProcessHelper
	{
		ProcessHelper()
		{
			DuplicateHandle(GetCurrentProcess(), GetCurrentProcess(), GetCurrentProcess(), &process, PROCESS_ALL_ACCESS, TRUE, 0);
		}

		void terminate()
		{
			ClipCursor(NULL);
			TerminateProcess(process, 0);
		}

		void restart()
		{
			STARTUPINFOA startup;
			mem::memzero(startup);
			startup.cb = sizeof(startup);
			startup.dwFlags = STARTF_USESTDHANDLES;

			PROCESS_INFORMATION process;
			mem::memzero(process);

			CHAR buffer[MAX_PATH];
			GetModuleFileNameA(NULL, buffer, MAX_PATH);
			if (CreateProcessA(buffer, GetCommandLineA(), nullptr, nullptr, FALSE, NORMAL_PRIORITY_CLASS, nullptr, nullptr, &startup, &process))
			{
				terminate();
			}
		}

		HANDLE process;
	};

	struct ProcessEditor : public imgui::modules::Module
	{
		ProcessEditor(ProcessHelper& h) :helper(h)
		{
			switcher = Input::KEY_P;
		}

		virtual void process()
		{
			if (imgui::button("Terminate"))
			{
				helper.terminate();
			}

			if (imgui::button("Restart"))
			{
				helper.restart();
			}
		}

		const char* name() const { return "Process"; }
		const wchar_t* viewportName() const { return L"default"; }

		ProcessHelper& helper;
	};

	bool terminationPredicate()
	{
		int shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000);
		int ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000);
		int z = (GetAsyncKeyState('Z') & 0x8000);

		return shift && ctrl && z;
	}

	bool restartPredicate()
	{
		int shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000);
		int ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000);
		int r = (GetAsyncKeyState('R') & 0x8000);

		return shift && ctrl && r;
	}

	void ProcessTerminator::release()
	{
		exitthread = true;
		thread.join();
	}

	void ProcessTerminator::init()
	{
		static ProcessHelper helper;
		static ProcessEditor editor(helper);

		thread = launch_named_thread("ProcessTerminator", [this] 
		{
			while (!exitthread)
			{
				Sleep(100);

				if (terminationPredicate())
				{
					helper.terminate();
				}
				else if (restartPredicate())
				{
					helper.restart();
				}
			}		
		});
	}
#else		
	void ProcessTerminator::init() {}
	void ProcessTerminator::release() {}
#endif



}