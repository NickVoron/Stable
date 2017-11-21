#pragma once
#include "affector.h"

namespace CameraComponents
{

struct ThirdPerson : public Affector<CameraSystem::Affectors::ThirdPerson>
{
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ThirdPerson);

	struct Resource : public ResourceBase
	{
		void properties(ThirdPerson& component);
	};
};

}
