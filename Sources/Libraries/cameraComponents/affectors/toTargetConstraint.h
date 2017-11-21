#pragma once
#include "affector.h"

namespace CameraComponents
{

struct ToTargetConstraint : public Affector<CameraSystem::Affectors::ToTargetConstraint>
{
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ToTargetConstraint);
};

}
