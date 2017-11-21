#pragma once
#include "affector.h"

namespace CameraComponents
{

struct DistanceConstraint : public Affector<CameraSystem::Affectors::DistanceConstraint>
{
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(DistanceConstraint);

	struct Resource : public ResourceBase
	{
		void properties(DistanceConstraint& component);
	};
};

}
