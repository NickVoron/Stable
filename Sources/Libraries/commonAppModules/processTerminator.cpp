// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.