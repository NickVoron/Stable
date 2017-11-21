#pragma once

#include "appModule/library.include.h"
#include "viewports/library.include.h"

#include "camera.h"


namespace AppModules
{

	struct ScreenMouseHelper : public ModuleBase<ScreenMouseHelper, Loki::MakeTypelist<Camera>>
	{
		struct Params
		{
			Params();
			intersections::Ray mouseRay;
			intersections::Frustum mouseFrustum;
			bool frustumCalc;
			bool frustumDraw;
		};

		template<class Params>  void process(Params& p) { process(p.currentCalculatedCamera, p); }
	
		void process(const Viewports::CalculatedCamera& camera, Params& params);
	};

}																										