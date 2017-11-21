#include "oneAxis.h"

namespace CameraSystem {
	namespace Affectors {

		void OneAxis::apply(float dt, Affector::Context& ctx)
		{
			if (!ctx.target)
			{
				State& s = ctx.params.getStateRef();

				Vector3 hpb = s.GetHPB();

				const float inv = 1.0f / 270.0f;
				Vector3 ihbp = ctx.input.hpb() * inv;

				hpb.x += ihbp.x;
				hpb.y = -nm::PI_4;

				const float limit = nm::PI_2;
				hpb.y = nm::clamp(hpb.y, -limit, limit);
				s.SetHPB(hpb);
			}
		}

	}
}
