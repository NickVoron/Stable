#pragma once
// аффектор для перемещения вверх/вниз и влево/вправо

#include "..\affector.h"

namespace CameraSystem{
namespace Affectors{

	struct HorizVert : public Affector
	{
		virtual void apply(float dt, Affector::Context& ctx);
	};

}
}