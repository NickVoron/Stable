#pragma once
// �������� ��� ���������� ������ �� �������� ����

#include "around.h"

namespace CameraSystem{
namespace Affectors{

	struct ThirdPerson : public Around
	{
		ThirdPerson();
		virtual void apply(float dt, Affector::Context& ctx);
	};

}
} // end 