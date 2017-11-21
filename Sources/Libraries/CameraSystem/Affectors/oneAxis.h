#pragma once

#include "..\affector.h"

namespace CameraSystem {
	namespace Affectors {

		struct OneAxis : public Affector
		{
			virtual void apply(float dt, Affector::Context& ctx);
		};

	}
}