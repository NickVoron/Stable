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

/*
class DebugDrawingModuleGL : public DebugDrawingModuleBase, public ModuleBase<DebugDrawingModuleGL, Loki::MakeTypelist<OpenGL, OpenGLFrameBegin, Camera>, Loki::MakeTypelist<DebugDrawingFlusher, OpenGLFrameEnd>>
{
public:
	struct Params : public DebugDrawingModuleBase::Params{};

	template<class Params>  void init(Params& p)		{ DebugDrawingModuleBase::init(p);		}
	template<class Params>  void release(Params& p)	{ DebugDrawingModuleBase::release(p);	}
	template<class Params>  void process(Params& p)	{ DebugDrawingModuleBase::process(p);	}

	virtual gapi::Id gapiId() const { return gapi::GL; }
};

class DebugDrawingModuleGLES : public DebugDrawingModuleBase, public ModuleBase<DebugDrawingModuleGLES, Loki::MakeTypelist<OpenGLES, OpenGLESFrameBegin, Camera>, Loki::MakeTypelist<DebugDrawingFlusher, OpenGLESFrameEnd>>
{
public:
	struct Params : public DebugDrawingModuleBase::Params {};

	template<class Params>  void init(Params& p) {  }
	template<class Params>  void release(Params& p) {  }
	template<class Params>  void process(Params& p) { }

	virtual gapi::Id gapiId() const { return gapi::GL; }
};
*/

}