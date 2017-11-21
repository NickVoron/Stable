#include "plane.h"


namespace CameraSystem {
	namespace Affectors {

		void Plane::apply(float dt, Affector::Context& ctx)
		{
			const Vector3& inPos = ctx.input.pos();

			State& s = ctx.state();

			s.position += s.orientation.GetXAxis() * inPos.x*0.1f;
			s.position += s.orientation.GetZAxis() * inPos.z*0.1f;
			s.position.y = 80;
		}

	}
}
