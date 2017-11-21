#pragma once

#include "appModule/library.include.h"
#include "lightSources/library.include.h"
#include "imguiModules/library.include.h"

#include "settings.h"
#include "winInput.h"

namespace AppModules
{
	class DefaultLightSources : public ModuleBase<DefaultLightSources, Loki::MakeTypelist<WinInput, Settings>>
	{
	public:
		struct Params
		{
			Params();
			nm::Vector3 directionalDir;
			nm::Vector3 directionalColor;
			nm::Vector3 ambientColor;
			bool drawLightSources;
			bool lightFromCamera;
		};

		template<class Params> void init(Params& p)		{ params = &p; init(); }
		template<class Params> void process(Params& p)	{ process(p.dt, p.currentCalculatedCamera); }
		template<class Params> void release(Params& p)	{ release(); }

		DefaultLightSources();
		void init();
		void process(float dt, const Viewports::CalculatedCamera& camera);
		void release();

		light::Directional* directional;
		light::Ambient* ambient;

		Params* params;
	};

}