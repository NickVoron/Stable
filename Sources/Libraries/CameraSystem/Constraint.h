#pragma once
// ����������� ������ �� ����� ���� ���������.

#include <string>
#include "params.h"
#include "target.h"

namespace CameraSystem
{

	class Constraint
	{
	public:
		std::string name;

		Constraint() {};
		virtual ~Constraint() {};

		// ��������� �����������, ������� false - ���� �� ���������� � ���������� �� ����� ���������
		virtual bool Apply(float dt, Params& cam_params, const Target* target) = 0;

	protected:

	};

}; // end namespace CameraSystem