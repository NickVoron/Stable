#pragma once
// Ограничение камеры по каком либо параметру.

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

		// применить ограничение, вернуть false - если не получилось и необзодимо по шагам разобрать
		virtual bool Apply(float dt, Params& cam_params, const Target* target) = 0;

	protected:

	};

}; // end namespace CameraSystem