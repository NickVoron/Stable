#pragma once
#include "appModule/library.include.h"

//#include "fbxImport/library.include.h"


namespace AppModules
{

	class FBX_DX11 : public ModuleBase<FBX_DX11>
	{
	public:
		template<class Params>  void init(Params& p) { }
		template<class Params>  void release(Params& p)	{  }
	};

}