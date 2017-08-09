// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "appModule/library.include.h"
#include "debugDrawing/library.include.h"
#include "gapi/library.include.h"

#include "d3d9/direct3d9.h"
#include "d3d11/direct3d11.h"
#include "opengl/opengl.h"
#include "opengles/opengles.h"

#include "resourceManager.h"
#include "camera.h"
#include "screenLog.h"
#include "debugDrawingFlusher.h"


namespace AppModules
{

struct DebugDrawingModuleBase
{
	struct Params
	{
		Params();

		const char* debugDrawShader;
		const char* debugFont;
		bool drawCoordinatesGrid;
	};

	template<class Params>  void init(Params& p)		{ init(p, gapiId());				}
	template<class Params>  void release(Params& p)	{ destroy(p);						}
	template<class Params>  void process(Params& p)	{ process(p.drawCoordinatesGrid);	}

	virtual gapi::Id gapiId() const = 0;

	void init(Params& params, gapi::Id id);
	void destroy(Params& params);
	void process(bool drawCoordinatesGrid);
};

class DebugDrawingModule9 : public DebugDrawingModuleBase, public ModuleBase<DebugDrawingModule9, Loki::MakeTypelist<Direct3D9FrameBegin, DriverD3D9, ResourcesManager, Camera> , Loki::MakeTypelist<DebugDrawingFlusher, ScreenLog9, Direct3D9FrameEnd>>
{
public:
	struct Params : public DebugDrawingModuleBase::Params{};

	template<class Params>  void init(Params& p)		{ DebugDrawingModuleBase::init(p);		}
	template<class Params>  void release(Params& p)	{ DebugDrawingModuleBase::release(p);	}
	template<class Params>  void process(Params& p)	{ DebugDrawingModuleBase::process(p);	}

	virtual gapi::Id gapiId() const { return gapi::DX9; }
};

class DebugDrawingModule11 : public DebugDrawingModuleBase, public ModuleBase<DebugDrawingModule11, Loki::MakeTypelist<DriverD3D11, Direct3D11FrameBegin, Camera>, Loki::MakeTypelist<DebugDrawingFlusher, Direct3D11FrameEnd>>
{
public:
	struct Params : public DebugDrawingModuleBase::Params{};

	template<class Params>  void init(Params& p)		{ DebugDrawingModuleBase::init(p);		}
	template<class Params>  void release(Params& p)	{ DebugDrawingModuleBase::release(p);	}
	template<class Params>  void process(Params& p)	{ DebugDrawingModuleBase::process(p);	}

	virtual gapi::Id gapiId() const { return gapi::DX11; }
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