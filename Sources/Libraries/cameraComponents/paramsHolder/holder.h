#pragma once

#include "componentModel/library.include.h"
#include "commonComponents/library.include.h"

#include "CameraSystem/library.include.h"
#include "cameraControllers/library.include.h"


namespace CameraComponents
{
	class SimpleTarget : public CameraSystem::Target
	{
	public:
		virtual State getState() const 
		{ 
			return state; 
		}

		State state;
	};

	class CameraParamHolder : public UpdatableComponent<CameraParamHolder>
	{
	public:

		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(CameraParamHolder);

		struct Resource : public ResourceBase 
		{
			void properties();
			Vector3 position;
			Vector3 hpb;
			float nearDist;
			float farDist;
			float verticalFov;
			float aspectRatio;
		};

		CameraParamHolder();
		~CameraParamHolder();

		void update(float dt);		
		void linker();

		typedef CameraSystem::UserCamera<CameraSystem::Affectors::VehicleView> VehicleCamera;

		//CameraSystem::ControlledCamera<CameraSystem::AroundCamera, CameraSystem::StandardAroundController> camera;
		CameraSystem::ControlledCamera<VehicleCamera, CameraSystem::NullController> camera;
		
		const Position* position;
	};
}

