#pragma once

#include "appModule/library.include.h"
#include "imgui/library.include.h"

#include "winInput.h"
#include "d3d9/direct3d9.h"
#include "d3d11/direct3d11.h"
#include "resourceManager.h"
//#include "renderTargetsManager.h"
#include "logs.h"
#include "camera.h"

namespace AppModules
{
	struct ImguiBase
	{
		void init();
		void process(float dt);
		void release();
	};

	template<class Prologue, class Epilogue>
	struct ImguiBaseT : public ImguiBase, public ModuleBase<ImguiBaseT<Prologue, Epilogue>, Prologue, Epilogue>
	{
		template<class Params>  void init(Params& p) { ImguiBase::init(); }
		template<class Params>  void process(Params& p) { ImguiBase::process(p.dt); }
		template<class Params>  void release(Params& params) { ImguiBase::release(); }
	};

	//struct Imgui9 : public ImguiBaseT<Loki::MakeTypelist<Camera, RenderTargetsManager9, ResourcesManager, WinInput, DriverD3D9, Logs>, Loki::MakeTypelist<Direct3D9FrameEnd>>
	struct Imgui9 : public ImguiBaseT<Loki::MakeTypelist<Camera, ResourcesManager, WinInput, DriverD3D9, Logs>, Loki::MakeTypelist<Direct3D9FrameEnd>>
	{
	};

	//struct Imgui11 : public ImguiBaseT<Loki::MakeTypelist<Camera, RenderTargetsManager11, ResourcesManager, WinInput, Direct3D11FrameBegin, DriverD3D11, Logs>, Loki::MakeTypelist<Direct3D11FrameEnd>>
	struct Imgui11 : public ImguiBaseT<Loki::MakeTypelist<Camera, ResourcesManager, WinInput, Direct3D11FrameBegin, DriverD3D11, Logs>, Loki::MakeTypelist<Direct3D11FrameEnd>>
	{
	};
}