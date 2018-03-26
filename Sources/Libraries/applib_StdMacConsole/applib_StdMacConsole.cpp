// Copyright (C) 2018 Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "appModule/library.include.h"
#include "applib_Main/library.include.h"
#include "commonAppModules/library.include.h"
#include "common/thread_utils.h"

extern void init(const std::vector<std::string>& commandLine);
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

		template<class Params> void init(Params& params) { ::init(params.commandLine); }
		template<class Params> void process(Params& params) { ::appmain(params.commandLine); }
	};


	typedef Loki::MakeTypelist<
		Locale,
		Autorun,
		Logs,
		QtLog,
		LogsUpdater,
		UTPNet,
		
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

int main(int argc, char* argv [])
{
    return common::main(argc, argv);
}












#endif




// Copyright (C) 2018 Denis Netakhin <denis.netahin@yandex.ru>
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