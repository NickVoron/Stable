#include "forward.h"

namespace CameraSystem{
namespace Affectors{

void Forward::apply(float dt, Affector::Context& ctx)
{
	float move = ctx.input.pos().z;
	if(move == 0.0f) return;
	
	State& s = ctx.params.getStateRef();

	// двигаем
	Vector3 v;

	// если есть цель
	if(ctx.target)
	{
		// получить вектор на цель
		v = ctx.target->state().position - s.position;

		float len = v.Magnitude();
		if(len != 0) // зашита
		{
			v.Normalize();
		}
		else
		{
			v = s.orientation.GetZAxis();
		}

		// установить ориентацию на цель в констрейнте...
	}
	else
	{
		v =s.orientation.GetZAxis();
	}

	s.position += v * move * dt;
}

}
}
