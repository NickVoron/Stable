#include "heightMapConstraint.h"

namespace CameraSystem{
namespace Affectors{
							   
HeightMapConstraint::Data::Data()
{
	height = 0;
}

void HeightMapConstraint::Data::range(Vector3& v)
{
	float zero = height ? height->height(v.x, v.z) : 0.0f;
	if (zero > v.y) v.y = zero;		
	h.RangeIt(v.y);
}

void HeightMapConstraint::apply(float dt, Affector::Context& ctx)
{
	data.range(ctx.params.getStateRef().position);
}

}
}