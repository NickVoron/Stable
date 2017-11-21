#include "forward.h"

namespace CameraSystem{
namespace Affectors{

void Forward::apply(float dt, Affector::Context& ctx)
{
	float move = ctx.input.pos().z;
	if(move == 0.0f) return;
	
	State& s = ctx.params.getStateRef();

	// �������
	Vector3 v;

	// ���� ���� ����
	if(ctx.target)
	{
		// �������� ������ �� ����
		v = ctx.target->state().position - s.position;

		float len = v.Magnitude();
		if(len != 0) // ������
		{
			v.Normalize();
		}
		else
		{
			v = s.orientation.GetZAxis();
		}

		// ���������� ���������� �� ���� � �����������...
	}
	else
	{
		v =s.orientation.GetZAxis();
	}

	s.position += v * move * dt;
}

}
}
