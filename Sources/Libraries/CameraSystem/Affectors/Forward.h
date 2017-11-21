#pragma once
// аффектор для движения вперёд назад, если есть цель, то двигается к ней

#include "..\affector.h"

namespace CameraSystem{
namespace Affectors{

	struct Forward : public Affector
	{
		virtual void apply(float dt, Affector::Context& ctx);
	};

}
} 