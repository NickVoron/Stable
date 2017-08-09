// Copyright (C) 2014-2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "CameraSystem/library.include.h"
#include "inputLib/library.include.h"

namespace CameraSystem
{
	struct StandardWSADController : public BaseCameraController
	{
		StandardWSADController();
		void update(CameraSystem::UserCameraBase& camera, float dt);

		float moveSpeed;
	};

	struct StandardAroundController : public BaseCameraController
	{
		StandardAroundController();
		void update(CameraSystem::UserCameraBase& camera, float dt);

		bool invertX;
		bool invertY;
		Input::Action pitchRotator;
		Input::Action headingRotator;
		float moveSpeed;
		float rotateSpeed;
	};

	struct NullController : public BaseCameraController
	{
		void update(CameraSystem::UserCameraBase& camera, float dt) {};
	};

	class ViewportCameras : public CameraSwitcher
	{
	public:
		ViewportCameras();

		void external(UserCameraBase* camera, bool interpolate = true);
		void target(const Vector3& targetPos);
		void setAspect(float aspect);

		virtual void update(float dt);

	private:		
		enum Mode
		{
			FREE_CAMERA,
			TARGETED,
			EXTERNAL,
		};

		ControlledCamera<FreeCamera, StandardWSADController> freecam;
		ControlledCamera<AroundCamera, StandardAroundController> targeted;
		UserCameraBase* externalCamera;

		Mode previousMode;
		Mode currentMode;
	};
}



// Copyright (C) 2014-2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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