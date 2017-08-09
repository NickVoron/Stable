// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "camera.h"

#include "settingsRegistry/library.include.h"
#include "debugDrawing/library.include.h"
#include "defaultLogs/library.include.h"
#include "newmath/library.include.h"
#include "WindowsGUI/library.include.h"
#include "inputLib/library.include.h"
#include "imgui/library.include.h"
#include "imguiModules/library.include.h"

namespace AppModules
{



struct CameraEditor : public imgui::modules::Module
{
	CameraEditor(Camera::Params& p) :params(p)
	{
		switcher = Input::KEY_C;
	}

	virtual void process();
	const char* name() const { return "Camera"; }
	const wchar_t* viewportName() const	{ return L"default"; }

	Camera::Params& params;
};

void CameraEditor::process()
{
	if (!CameraSystem::cameras().empty())
	{
		CameraSystem::Params& camera = CameraSystem::cameras().first()->params;

		float fovV = nm::degrees(camera.getFovV());
		imgui::slider("Vertical FOV", 5.0f, 180.0f, fovV);
		camera.setFovV(nm::radians(fovV));

		float fovH = nm::degrees(camera.getFovH());
		imgui::slider("Horizontal FOV", 5.0f, 180.0f, fovH);
		camera.setFovH(nm::radians(fovH));

		static Base::Interval<float> nearLimits(0.01f, 10.0f);
		static Base::Interval<float> maxFarLimits(100.0f, 2048.0f);
		static Base::Interval<float> speedLimits(1.0f, 10.0f);

		Base::Interval<float>& planes = camera.getPlanesRef();

		imgui::slider("Near Plane", nearLimits, planes.minValue);
		planes.normalize();

		imgui::slider("Far Plane", Base::Interval<float>(nearLimits.maxValue, params.farPlaneMax), planes.maxValue);
		planes.normalize();

		imgui::slider("Max Far Plane", maxFarLimits, params.farPlaneMax);
		imgui::slider("Move Speed", speedLimits, params.cameraMoveSpeed);
	}
}




Camera::Params::Params()
{
	farPlaneMax = 512.0f;
	cameraMoveSpeed = 2.5f;
	xpadControl = false;
}

LRESULT Camera::CameraWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WindowsGUI::Window* window = 0;
	LONG_PTR data = GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if(data)
	{
		window = (WindowsGUI::Window*)data;
	}

	switch (message)
	{
	case WM_ACTIVATE: 
		{
			if(wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE)
			{
				window->activate();				
			}			
			break;
		}
	}

	return 0;
}


void Camera::save(Params& params)
{
	SettingsRegistry::add("cameraFarMax", params.farPlaneMax);
	SettingsRegistry::add("cameraMoveSpeed", params.cameraMoveSpeed);
}

void Camera::init_data(Window::Params& windowParams, Params& params)
{
	static CameraEditor editor(params);

	windowParams.windowProcs.add(&CameraWndProc);

	SettingsRegistry::get("cameraFarMax", params.farPlaneMax);
	SettingsRegistry::get("cameraMoveSpeed", params.cameraMoveSpeed);
}


void Camera::update(float dt, Params& params)
{
	if (!Viewports::viewports.empty())
	{
		Viewports::Viewport* vp = Viewports::viewports.first();

		vp->camera.update(dt);

		params.currentCalculatedCamera = vp->calculateCamera();

		draw();
	}
}


void Camera::draw()
{




}

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