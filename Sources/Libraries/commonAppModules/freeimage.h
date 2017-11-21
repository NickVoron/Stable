#pragma once
#include "appModule/library.include.h"

#include "libfreeimage/library.include.h"


namespace AppModules
{

	class FreeImage : public ModuleBase<FreeImage>
	{
	public:
		template<class Params>  void init(Params& p)	
		{
#ifdef WIN32
			FreeImage_Initialise(true); 
#endif
		}
		template<class Params>  void release(Params& p)	
		{ 
#ifdef WIN32
			FreeImage_DeInitialise(); 
#endif
		}
	};

}