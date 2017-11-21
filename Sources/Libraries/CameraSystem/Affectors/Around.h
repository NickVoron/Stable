#pragma once
// �������� ��� �������� ������ ����

#include "../affector.h"

namespace CameraSystem{
namespace Affectors{

	struct Around : public Affector
	{
		Around();

		virtual void apply(float dt, Affector::Context& ctx);

		RangedValue<float>	distance;
		float	height_offset;	// ������ �������� ����� ����������� �� ������ ����
	};

}
} // end 