// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "appModule/library.include.h"
#include "applib_Main/library.include.h"
#include "commonAppModules/library.include.h"

extern int appmain(const std::vector<std::string>& commandLine);

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

	return appmain<Prologue, 1, TESTS_ENABLED>(&setup, argc, argv);
}



// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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