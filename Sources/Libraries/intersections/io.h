#pragma once

#include "shapes/shapes.h"
#include "shapes/obb.h"

namespace intersections
{
	std::ostream& operator<<(std::ostream& os, const AABB& obj);
	std::ostream& operator<<(std::ostream& os, const OBB& obj);
	std::ostream& operator<<(std::ostream& os, const Sphere& obj);
	std::ostream& operator<<(std::ostream& os, const Ray& obj);
	std::ostream& operator<<(std::ostream& os, const Line& obj);
	std::ostream& operator<<(std::ostream& os, const Segment3& obj);
	std::ostream& operator<<(std::ostream& os, const Capsule3& obj);
}





