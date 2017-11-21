#pragma once
#include "newmath/library.include.h"
#include "math/library.include.h"

namespace intersections
{
	bool test(const nm::plane& plane0, const nm::plane& plane1, const nm::plane& plane2, Vector3& result);
}//