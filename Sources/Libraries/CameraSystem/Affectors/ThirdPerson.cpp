#include "thirdPerson.h"

namespace CameraSystem{
namespace Affectors{

ThirdPerson::ThirdPerson()
{
//	asimuth_speed_k1 = 0;
//	height = 0;
}

void ThirdPerson::apply(float dt, Affector::Context& ctx)
{
	if(ctx.target)
	{
		RangedValue<float>	rise;
		RangedValue<float>	asimuth;
		//RangedValue<float>	distance;

		asimuth.SetCycleMode(true); // ������ ��������
		asimuth.SetMin(0);
		asimuth.SetMax(nm::PI * 2);
		rise.SetMin(5 * nm::PI_180); // �� 5 �� 175 ��������
		rise.SetMax(nm::PI - 5 * nm::PI_180);
		rise = nm::PI_2; // �������� ���������� �� ����������� �����
// 		distance.SetMin(1);
// 		distance.SetMax(10);

		float inv = 1.0f / 270.0f;
		// ��������� ����
		distance -= ctx.input.pos().z * inv; // ����� ��������� ���������, ������� -
		rise += ctx.input.hpb().y * inv;

		// ������ ����� ������
		float radius = ctx.target->radius();
		if(distance < radius)
			distance = radius;

		//�������� ��������� ����� 360?...

/*		Quaternion qq;
		float a = 3.14f * 2;
		SetQAsimuth(qq, a);
		float b = GetQAsimuth(qq);
		float d = b - a;
*/
		Vector3 dir(0, 0, distance);
		Quaternion  xRot; xRot.Set(rise - nm::PI_2, Vector3::xAxis);
		
		// ������� ������� ����
		float cur_asimuth = ctx.target->state().orientation.GetZAxis().GetSphereAsimuthAngle();
//		if(!asimuth_speed_k1)
		{
			asimuth = cur_asimuth;
		}
/*		else
		{
			float a_delta = cur_asimuth - asimuth;
			asimuth += a_delta * asimuth_speed_k1 * dt;
		}
*/
		Quaternion  yRot; yRot.Set(asimuth, Vector3::yAxis);
		Quaternion q(yRot * xRot);

		// ��������� ������
		dir = Base::MathUtils::postRotate(dir, q);

		State& s = ctx.params.getStateRef();
		s.position = ctx.target->state().position + Vector3(0, height_offset, 0) - dir;
		s.orientation = q;
	}
}

}
}