#pragma once
// аффектор для вращения вокруг цели

#include "../affector.h"

namespace CameraSystem{
namespace Affectors{

	struct Around : public Affector
	{
		Around();

		virtual void apply(float dt, Affector::Context& ctx);

		RangedValue<float>	distance;
		float	height_offset;	// высота смещения точки нацеливания от центра цели
	};

}
} // end 