#pragma once
#include "affector.h"

namespace CameraComponents
{

struct Around : public Affector<CameraSystem::Affectors::Around>
{
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(Around);

	struct Resource : public ResourceBase
	{
		void properties(Around& component);
	};
};

}
