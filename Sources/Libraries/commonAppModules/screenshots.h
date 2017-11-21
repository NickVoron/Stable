#pragma once
#include "appModule/library.include.h"
#include "winInput.h"
#include "d3d9/direct3d9.h"

namespace AppModules
{

	class Screenshots : public ModuleBase<Screenshots, Loki::MakeTypelist<WinInput, Direct3D9>>
	{
	public:
		template<class Params>  void init(Params& p)			{	init();		}
		template<class Params>  void after_process(Params& p)	{	after_process();	}

		void init();
		void after_process();
	};

}