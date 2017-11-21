#pragma once
// аффектор для вращения камеры без цели

#include "../affector.h"

namespace CameraSystem{
namespace Affectors{

	struct FreeRotator : public Affector
	{
		virtual void apply(float dt, Affector::Context& ctx);
	};

}
}