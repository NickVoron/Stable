#include "around.h"

namespace CameraSystem{
namespace Affectors{

using namespace Base;

Around::Around() : distance(1.0f, 100.0f)
{
	height_offset = 0;
}

void Around::apply(float dt, Affector::Context& ctx)
{
	if(ctx.target)
	{
		float inv = 1.0f / 270.0f;
		
		Vector3 ihpb = ctx.input.hpb() * inv;
		ihpb.z = 0.0f;
		
		distance.SetMin(ctx.target->radius());
		distance -= ctx.input.pos().z;


		State& s = ctx.params.getStateRef();
		Vector3 hpb = s.GetHPB();
		hpb.x += ihpb.x;
		hpb.y -= ihpb.y;
 
 		const float limit = nm::PI_2 - 0.00001f;
 		hpb.x = nm::simplify_angle(hpb.x);
 		hpb.y = nm::clamp( hpb.y, -limit, limit );
		
		s.SetHPB(hpb);
		s.position = ctx.target->state().position + Vector3(0, height_offset, 0) - s.orientation.GetZAxis() * distance;		
	}
}

}
}