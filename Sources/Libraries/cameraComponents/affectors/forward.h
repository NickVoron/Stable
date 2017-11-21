#pragma once
#include "affector.h"

namespace CameraComponents
{

struct Forward : public Affector<CameraSystem::Affectors::Forward>
{
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(Forward);
};

}
