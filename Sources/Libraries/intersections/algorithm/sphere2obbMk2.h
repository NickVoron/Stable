#pragma once
#include "../shapes/shapes.h"
#include "../shapes/obb.h"

namespace intersections
{
	void nearestPointMk2(const Vector3& form, const OBB& obb, Vector3& point);


}