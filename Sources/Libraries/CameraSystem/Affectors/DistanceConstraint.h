#pragma once
// Ограничение мнимальной и максимальной дистанции до цели

#include "../affector.h"

namespace CameraSystem{
namespace Affectors{

	struct DistanceConstraint: public Affector
	{
		DistanceConstraint();

		virtual void apply(float dt, Affector::Context& ctx);

		Range<float> range;
	};

}
} // end namespace Ca