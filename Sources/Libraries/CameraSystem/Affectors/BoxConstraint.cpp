#include "boxConstraint.h"

namespace CameraSystem{
namespace Affectors{

BoxConstraint::Data::Data()
{
	enabledX = enabledY = enabledZ = false;
}

void BoxConstraint::Data::range(Vector3& v)
{
	if ( enabledX ) x.RangeIt( v.x );
	if ( enabledY ) y.RangeIt( v.y );
	if ( enabledZ ) z.RangeIt( v.z );
}

void BoxConstraint::apply(float dt, Affector::Context& ctx)
{
	data.range(ctx.params.getStateRef().position);
}

}
}