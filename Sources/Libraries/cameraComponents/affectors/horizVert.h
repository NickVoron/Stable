#pragma once
#include "affector.h"

namespace CameraComponents
{

struct HorizVert : public Affector<CameraSystem::Affectors::HorizVert>
{
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(HorizVert);
};

}
