#pragma once
#include "paramsHolder/include.h"

namespace CameraComponents
{

struct CameraSetter : public UpdatableComponent<CameraSetter>
{
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(CameraSetter);

	void linker();

	void update(float dt);
	void onDeactivate();

	CameraParamHolder* paramHolder;
};

}
