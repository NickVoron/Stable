#pragma once

#include "vector.h"

namespace nm
{
	template<typename scalar>
	struct ray3_t 
	{
		vector3_t<scalar> origin;
		vector3_t<scalar> direction;
	};
}
