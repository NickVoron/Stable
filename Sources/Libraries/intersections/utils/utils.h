#pragma once
#include "math/library.include.h"
#include "../shapes/shapes.h"
#include "../shapes/aabb.h"

namespace intersections
{
	struct AABB;

	void convert(const Quaternion& orientation, nm::Vector3* axis);
	void convert(const nm::Vector3 axis[3], Quaternion& orientation);

	void computeBoundBox(const nm::Vector3* vertices, unsigned int size, intersections::AABB& bb);

	AABB aabb(const Sphere& sphere);
	AABB aabb(const nm::Vector3& position, float extent);

}//