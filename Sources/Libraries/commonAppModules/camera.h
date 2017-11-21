#pragma once

#include "appModule/library.include.h"
#include "cameraSystem/library.include.h"
#include "viewports/library.include.h"

#include "timer.h"
#include "winInput.h"
#include "settings.h"

#include "cameraControllers/library.include.h"


namespace AppModules
{

class Camera : public ModuleBase<Camera, Loki::MakeTypelist<Timer, WinInput, Settings>>
{
public:	
	struct Params
	{
		Params();
		
		Viewports::CalculatedCamera currentCalculatedCamera;

		float farPlaneMax;
		float cameraMoveSpeed;
		bool xpadControl;
	};

	static LRESULT CameraWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	template<class Params>  void init(Params& p)		{ init_data(p, p); }
	template<class Params>  void process(Params& p)	{ update(p.dt, p); }
	template<class Params>  void release(Params& p)	{ save(p); }

	void update(float dt, Params& params);
	void draw();

	void init_data(Window::Params& windowParams, Params& params);
	void save(Params& params);
};

}