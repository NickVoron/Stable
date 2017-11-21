#pragma once

#include "settingsRegistry/library.include.h"

#include "direct3d9.h"
#include "../winInput.h"

namespace AppModules
{

/*
struct D3DCoreEditor : public imgui::modules::Module
{
	D3DCoreEditor(Direct3D9FrameBegin::Params& p):params(p){}
	virtual void process();
	virtual const char* name() const;

	Direct3D9FrameBegin::Params& params;
};
													 
void D3DCoreEditor::process()
{
	imgui::color_d3d("Clear Target", params.d3dClear_Color);
}

const char* D3DCoreEditor::name() const
{
	return "Direct3D9 Core";
}
*/


//
//
//
Direct3D9FrameBegin::Params::Params()
{
	color	= Vector4(0.0f, 0.425f, 0.7f, 1.0f);
	depth	= 1.0f;
	stencil	= 0;

	clearDepth		= true;
	clearColor		= true;
	clearStencil	= false;
}

/*

DWORD Direct3D9FrameBegin::Params::flags() const
{
	DWORD res = 0;

	if(clearColor)		res |= D3DCLEAR_TARGET;
	if(clearDepth)		res |= D3DCLEAR_ZBUFFER;
	if(clearStencil)	res |= D3DCLEAR_STENCIL;

	return res;
}*/

void Direct3D9::create(Window::Params& windowParams, Params& params)
{
	//dx11::DeviceCreate();

	windowParams.createWindows(params.windows);
}

void Direct3D9::release()
{
	//dx11::DeviceRelease(); 
}

void Direct3D9FrameBegin::create(Params& params)
{
	//static D3D11CoreEditor editor(params);
}

void Direct3D9FrameBegin::process(WindowsGAPI::Windows9& windows)
{
	windows.clear();
}

void Direct3D9FrameEnd::process(WindowsGAPI::Windows9& windows)
{
	windows.present();
}

}
