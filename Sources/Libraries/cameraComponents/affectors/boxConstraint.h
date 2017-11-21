#pragma once
#include "affector.h"

namespace CameraComponents
{

struct BoxConstraint : public Affector<CameraSystem::Affectors::BoxConstraint>
{
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(BoxConstraint);

	struct Resource : public ResourceBase
	{
		void properties(BoxConstraint& component);
	};
};

}
