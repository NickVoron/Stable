#include "aabb.h"
#include "../utils/utils.h"

namespace intersections
{

	void AABB::collapse()
	{
		bmin = nm::Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
		bmax = nm::Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	}

	AABB AABB::operator+(const AABB& a2)
	{
		AABB result(*this);
		result.combine(a2);
		return result;
	}

	void AABB::push(const nm::Vector3& point)
	{
		bmin.x = std::min(bmin.x, point.x);
		bmin.y = std::min(bmin.y, point.y);
		bmin.z = std::min(bmin.z, point.z);

		bmax.x = std::max(bmax.x, point.x);
		bmax.y = std::max(bmax.y, point.y);
		bmax.z = std::max(bmax.z, point.z);
	}

	void AABB::combine(const AABB& aabb)
	{
		bmin.x = std::min(bmin.x, aabb.bmin.x);
		bmin.y = std::min(bmin.y, aabb.bmin.y);
		bmin.z = std::min(bmin.z, aabb.bmin.z);

		bmax.x = std::max(bmax.x, aabb.bmax.x);
		bmax.y = std::max(bmax.y, aabb.bmax.y);
		bmax.z = std::max(bmax.z, aabb.bmax.z);
	}

	
	bool AABB::operator==(const AABB& aabb) const
	{
		return nm::magnitude(bmin - aabb.bmin) < 0.00001f && nm::magnitude(bmax - aabb.bmax) < 0.00001f;
	}

	void AABB::transform(const State& state, float scale)
	{
		Quad xn = quadXn();
		Quad xp = quadXp();

		xn.transform(state, scale);
		xp.transform(state, scale);

		AABB xnAABB;
		computeBoundBox(xn.vertices, Quad::VERTICES_COUNT, xnAABB);


		AABB xpAABB;
		computeBoundBox(xp.vertices, Quad::VERTICES_COUNT, xpAABB);

		xnAABB.combine(xpAABB);

		*this = xnAABB;
	}

}