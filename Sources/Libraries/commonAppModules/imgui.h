// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "appModule/library.include.h"
#include "imgui/library.include.h"

#include "winInput.h"
#include "d3d9/direct3d9.h"
#include "d3d11/direct3d11.h"
#include "resourceManager.h"
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

	
	struct Imgui9 : public ImguiBaseT<Loki::MakeTypelist<Camera, ResourcesManager, WinInput, DriverD3D9, Logs>, Loki::MakeTypelist<Direct3D9FrameEnd>>
	{
	};

	
	struct Imgui11 : public ImguiBaseT<Loki::MakeTypelist<Camera, ResourcesManager, WinInput, Direct3D11FrameBegin, DriverD3D11, Logs>, Loki::MakeTypelist<Direct3D11FrameEnd>>
	{
	};
}



// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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