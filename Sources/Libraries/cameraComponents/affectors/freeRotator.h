#pragma once
#include "affector.h"

namespace CameraComponents
{

struct FreeRotator : public Affector<CameraSystem::Affectors::FreeRotator>
{
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(FreeRotator);
};

}
