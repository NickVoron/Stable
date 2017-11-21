#pragma once
#include "../shapes/shapes.h"
#include "../shapes/obb.h"
#include "../shapes/triangle.h"

namespace intersections
{

	bool test(const AABB& aabb0, const AABB& aabb1);

	bool test(const Ray& ray, const AABB& obb, Vector3& point);
	bool test(const Ray& ray, const OBB& obb);
	bool test(const Ray& ray, const OBB& obb, Vector3& point);
	bool test(const Ray& ray, const Sphere& sphere);
	bool test(const Ray& ray, const Sphere& sphere, Vector3& point);
	bool test(const Ray& ray, const Quad& quad);
	bool test(const Ray& ray, const Triangle& trg);
	bool test(const Segment3& ray, const Triangle& trg);

	bool test(const Ray& ray, const Triangle& trg, Vector3& point);
	bool test(const Ray& ray, const Quad& quad, Vector3& point);
	bool test(const Segment3& ray, const Triangle& trg, Vector3& point);

	template<int vcount>
	bool test(const Ray& ray, const Prysm<vcount>& prysm)
	{
		return true;
	}


}//