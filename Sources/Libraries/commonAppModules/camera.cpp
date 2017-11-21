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
//
//
//
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

//
//
//
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

// обновить камеру
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
// 	for (const auto& camera : CameraSystem::cameras())
// 	{
// 		DebugDrawing::camera(camera, 0.25f, false); 		
// 	}
}

}