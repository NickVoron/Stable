#include "horizVert.h"

namespace CameraSystem{
namespace Affectors{

void HorizVert::apply(float dt, Affector::Context& ctx)
{
	const Vector3& pos = ctx.input.pos();
	if (pos.x == 0.0f && pos.y == 0.0f)
		return;
	
	State& s = ctx.state();
	s.position += (s.orientation.GetXAxis() * pos.x + s.orientation.GetYAxis() * pos.y) * dt;
}

}
}
