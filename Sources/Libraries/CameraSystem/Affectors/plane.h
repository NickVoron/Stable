#pragma once

#include "..\affector.h"

namespace CameraSystem {
	namespace Affectors {

		struct Plane : public Affector
		{
			virtual void apply(float dt, Affector::Context& ctx);
		};

	}
}