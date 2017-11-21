#include "distanceConstraint.h"

#undef min
#undef max

namespace CameraSystem{
namespace Affectors{

DistanceConstraint::DistanceConstraint()
:range(0.0f, 10.0f)
{
}


// ��������� �������� � ���������� ������, ������� false - ���� ��������� ����������� �� �����
void DistanceConstraint::apply(float dt, Affector::Context& ctx)
{
	if(ctx.target)
	{
		Vector3 to_target = ctx.target->state().position - ctx.params.getStateRef().position;
		float old_dist = to_target.Magnitude();
		
		if(old_dist == 0) return;

		// ����������
		float radius = ctx.target->radius();
		range.Normalize();
		
		float dist = nm::clamp(old_dist, std::max(range.minValue, radius), std::max(range.maxValue, radius));
		float move = dist - old_dist;

		// ���� ��������� ������, ����������
		if(move != 0)
		{
			to_target /= old_dist; // �����������
			ctx.params.getStateRef().position -= to_target * move; // ����������� �� �������
		}
	}
}

}
}