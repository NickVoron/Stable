// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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
			Vector3 directionalDir;
			Vector3 directionalColor;
			Vector3 ambientColor;
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



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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