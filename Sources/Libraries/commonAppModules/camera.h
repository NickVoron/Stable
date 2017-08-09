// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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