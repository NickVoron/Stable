#include "toTargetConstraint.h"

namespace CameraSystem{
namespace Affectors{

void ToTargetConstraint::apply(float dt, Affector::Context& ctx)
{
	if(ctx.target)
	{
		Vector3 to_target = ctx.target->state().position - ctx.params.getData().state.position;

		if(to_target.MagnitudeSquared() > 0)
		{
			to_target = to_target.GetNormalized();
			ctx.params.getStateRef().orientation = Quaternion::GetUpOrientedDirection(to_target);
		}
	}
}

}
}
