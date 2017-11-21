#pragma once

#include "imgui/library.include.h"
#include "imguiModules/library.include.h"

#include "direct3d11.h"
#include "../winInput.h"

namespace AppModules
{

struct D3D11CoreEditor : public imgui::modules::Module
{
	D3D11CoreEditor(Direct3D11FrameBegin::Params& p):params(p)
	{
		//switcher = Input::KEY_C;
	}

	virtual void process()
	{
		imgui::color("Clear Target", params.color);
	}

	virtual const char* name() const
	{
		return "Direct3D11 Core";
	}

	Direct3D11FrameBegin::Params& params;
};

//
//
//
void Direct3D11::create(Window::Params& windowParams, Params& params)
{
	dx11::DeviceCreate();

	windows = &params.windows;
	windowParams.createWindows(params.windows);
}

void Direct3D11::release()
{
	dx11::VertexShaderUtilStorage::release();
	windows->closeWindows();
	dx11::DeviceRelease();
}

//
//
//
Direct3D11FrameBegin::Params::Params()
{
	color	= Vector4(0.0f, 0.425f, 0.7f, 1.0f);
	depth	= 1.0f;
	stencil	= 0;

	clearDepth		= true;
	clearColor		= true;
	clearStencil	= false;
}


void Direct3D11FrameBegin::create(Params& params)
{
	//static D3D11CoreEditor editor(params);
}

void Direct3D11FrameBegin::process(WindowsGAPI::Windows11& windows)
{
	windows.clear();
}

//
//
//
void Direct3D11FrameEnd::process(WindowsGAPI::Windows11& windows)
{
	windows.present();
}

}
