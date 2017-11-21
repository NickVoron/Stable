#pragma once
// аффектор для нацеленной камеры от третьего лица

#include "around.h"

namespace CameraSystem{
namespace Affectors{

	struct ThirdPerson : public Around
	{
		ThirdPerson();
		virtual void apply(float dt, Affector::Context& ctx);
	};

}
} // end 