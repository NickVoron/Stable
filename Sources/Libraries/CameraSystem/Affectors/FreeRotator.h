#pragma once
// �������� ��� �������� ������ ��� ����

#include "../affector.h"

namespace CameraSystem{
namespace Affectors{

	struct FreeRotator : public Affector
	{
		virtual void apply(float dt, Affector::Context& ctx);
	};

}
}