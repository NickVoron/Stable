#include "io.h"

#include "math/library.include.h"

namespace intersections
{
	std::ostream& operator<<(std::ostream& os, const AABB& obj)
	{
		return os << "AABB: min: " << Vector3(&obj.bmin.x) << " max: " << Vector3(&obj.bmax.x);
	}

	std::ostream& operator<<(std::ostream& os, const OBB& obj)
	{
		return os << "OBB: center: " << obj.center << " axis[0]: " << obj.axis[0] << " axis[1]: " << obj.axis[1] << " axis[2]: " << obj.axis[2] << " extent[0]: " << obj.extent[0] << " extent[1]: " << obj.extent[1] << " extent[2]: " << obj.extent[2];
	}

	std::ostream& operator<<(std::ostream& os, const Sphere& obj)
	{
		return os << "SPHERE: center: " << obj.center << " radius: " << obj.radius;
	}

	std::ostream& operator<<(std::ostream& os, const Ray& obj)
	{
		return os << "RAY: origin: " << obj.origin << " direction: " << obj.direction;
	}

	std::ostream& operator<<(std::ostream& os, const Line& obj)
	{
		return os << "LINE: origin: " << obj.origin << " direction: " << obj.direction;
	}

	std::ostream& operator<<(std::ostream& os, const Segment3& obj)
	{
		return os << "SEGMENT: p0: " << obj.p0 << " p1: " << obj.p1;
	}

	std::ostream& operator<<(std::ostream& os, const Capsule3& obj)
	{
		return os << "CAPSULE: segment: " << obj.segment << " radius: " << obj.radius;
	}
}




