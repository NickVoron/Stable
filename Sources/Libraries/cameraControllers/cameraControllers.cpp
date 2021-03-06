// Copyright (C) 2014-2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "cameraControllers.h"

namespace CameraSystem
{
	StandardWSADController::StandardWSADController()
	{
		moveSpeed = 2.5f;
	}

	void StandardWSADController::update(CameraSystem::UserCameraBase& camera, float dt)
	{
		using namespace Input;

		CameraSystem::FreeCamera* freecam = dynamic_cast<CameraSystem::FreeCamera*>(&camera);
		if (freecam)
		{
			if (keyboard(KEY_NUMPAD7, true))
			{
				Vector3 curpos = freecam->params.getState().position;
				curpos.x = -curpos.x;
				freecam->position(curpos);
			}

			if (keyboard(KEY_H, true))			freecam->position(Vector3::zero);
			if (keyboard(KEY_NUMPAD9, true))	freecam->headingAdd(nm::PI_2);
			if (keyboard(KEY_NUMPAD5, true))	freecam->lookTo(Vector3::zero);
		}
			

		float coef = 1.0f;
		float rotCoef = 1.0f;

		bool accel1 = keyboard(KEY_SHIFT) | xpad(0, XPAD_AXIS_LEFT_TRIGGER);
		bool accel2 = keyboard(KEY_CONTROL) | xpad(0, XPAD_BUTTON_LEFT_SHOULDER);

		bool rotorAccel1 = xpad(0, XPAD_AXIS_LEFT_TRIGGER);
		bool rotorAccel2 = xpad(0, XPAD_BUTTON_LEFT_SHOULDER);

		if (accel1 && accel2)
		{
			coef *= 100.0;
		}
		else
		{
			if (accel1) coef *= 10.0f;
			if (accel2) coef *= 100.0f;
		}

		if (rotorAccel1 && rotorAccel2)
		{
			rotCoef *= 20.0;
		}
		else
		{
			if (rotorAccel1) rotCoef *= 10.0f;
			if (rotorAccel2) rotCoef *= 20.0f;
		}

		float speed = moveSpeed * coef;

		camera.input.horizontal((value(KEY_D) - value(KEY_A)) * speed);
		camera.input.forward((value(KEY_W) - value(KEY_S)) * speed);
		camera.input.vertical((value(KEY_Q) - value(KEY_E)) * speed);
		
		camera.input.heading(delta(MOUSE_AXIS_X, MOD_MOUSE_BUTTON_RIGHT));
		camera.input.pitch(delta(MOUSE_AXIS_Y, MOD_MOUSE_BUTTON_RIGHT));



		camera.input.update(dt);
	}

	
	
	
	StandardAroundController::StandardAroundController()
	{
		invertY = false;
		invertX = false;

		moveSpeed = 0.1f;
		rotateSpeed = 2.5f;

		headingRotator.init(Input::MOUSE_AXIS_X, Input::MOD_MOUSE_BUTTON_RIGHT);
		pitchRotator.init(Input::MOUSE_AXIS_Y, Input::MOD_MOUSE_BUTTON_RIGHT);
	}

	void StandardAroundController::update(CameraSystem::UserCameraBase& camera, float dt)
	{
		using namespace Input;

		float pitchInv = invertY ? -1.0f : 1.0f;
		float headingInv = invertX ? -1.0f : 1.0f;

		float coef = 1.0f;

		bool accel1 = keyboard(KEY_SHIFT) | xpad(0, XPAD_AXIS_LEFT_TRIGGER);
		bool accel2 = keyboard(KEY_CONTROL) | xpad(0, XPAD_BUTTON_LEFT_SHOULDER);

		if (accel1 && accel2)
		{
			coef *= 100.0;
		}
		else
		{
			if (accel1) coef *= 10.0f;
			if (accel2) coef *= 100.0f;
		}

		float speed = moveSpeed * coef;

		camera.input.forward((value(KEY_W) - value(KEY_S) + delta(MOUSE_AXIS_Z)) * speed);
		
		
		
		camera.input.heading(rotateSpeed * headingInv * (headingRotator.delta()));
		camera.input.pitch(rotateSpeed * pitchInv * (pitchRotator.delta()));
		camera.input.update(dt);
	}



	
	
	
	ViewportCameras::ViewportCameras() : CameraSwitcher(freecam)
	{
		currentMode = FREE_CAMERA;
		previousMode = FREE_CAMERA;
	}

	void ViewportCameras::external(UserCameraBase* camera, bool interpolate)
	{
		if(camera)
		{
			previousMode = currentMode;
			currentMode = EXTERNAL;
			externalCamera = camera;
			activate(externalCamera, interpolate);
		}
		else if (currentMode == EXTERNAL)
		{
			ENFORCE(externalCamera);

			externalCamera = 0;

			if(previousMode == FREE_CAMERA)
			{
				currentMode = FREE_CAMERA;
				defaults();
			}
			else
			{
				currentMode = TARGETED;
				activate(&targeted, interpolate);
			}			
		}
	}
	
	void ViewportCameras::target(const Vector3& targetPos)
	{
		if (currentMode != TARGETED)
		{
			currentMode = TARGETED;
			targeted.focusTo(freecam.params.getState().position, targetPos);
			activate(&targeted, true);
		}
		else
		{
			Params tparams = targeted.params;
			tparams.lookTo(targetPos);
			animate(targeted.params, tparams);
			targeted.focusTo(targetPos);
		}
	}

	void ViewportCameras::setAspect(float aspect)
	{
		freecam.params.setAspect(aspect);
		targeted.params.setAspect(aspect);
	}

	void ViewportCameras::update(float dt)
	{
		if (currentMode == TARGETED && Input::keyboard(Input::KEY_ESCAPE, true))
		{
			currentMode = FREE_CAMERA;
			freecam.params.getStateRef() = targeted.params.getState();
			defaults();
		}

		CameraSwitcher::update(dt);
	}
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