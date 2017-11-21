#pragma once

#include "appModule/library.include.h"
#include "camera.h"

namespace AppModules
{

	struct Manipulators3D : public ModuleBase<Manipulators3D, Loki::MakeTypelist<Camera>>
	{
		template<class Params> void process(Params& p)	{ update(p.currentCalculatedCamera); }

		void update(const Viewports::CalculatedCamera& camera);
	};

}