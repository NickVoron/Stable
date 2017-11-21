#pragma once

#include "appModule/library.include.h"

namespace AppModules
{

	class Locale : public ModuleBase<Locale>
	{
	public:
		struct Params
		{
			Params()	{	localeName = "rus";	}

			const char* localeName;
		};

		template<class Params>  void init(Params& p)		{ setlocale(LC_ALL, p.localeName); };
	};

}