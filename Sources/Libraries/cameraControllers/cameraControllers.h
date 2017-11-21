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