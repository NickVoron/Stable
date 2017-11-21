#pragma once
#include <iostream>
#include <algorithm>

#include "math/base/Vector3.h"
#include "newmath/library.include.h"
#include "aabb.h"
#include <vector>

namespace intersections
{
	struct OBB
	{
	public:
		OBB(){}
		OBB(const Vector3& center, const Vector3 axis[3], const float extent[3]);
		OBB(const Vector3& center, const Vector3& axis0, const Vector3& axis1, const Vector3& axis2, const float extent0, const float extent1, const float extent2);
		OBB(const AABB& aabb);

		Vector3 center;
		Vector3 axis[3];
		float extent[3];

		void normalizeAxis() { axis[0].Normalize(); axis[1].Normalize(); axis[2].Normalize(); }

		Quad quadXn() const;
		Quad quadXp() const;

		Quad quadYn() const;
		Quad quadYp() const;

		Quad quadZn() const;
		Quad quadZp() const ;

		std::vector<Vector3>  getCorners() const;

		float boudingSphere();
	};

	template<int vcount>
	struct Prysm
	{
		Prysm() :baseVerticesCount(vcount){}

		void ortho(float height)
		{
			nm::cross(base[1] - base[0], base[2] - base[0], edge);
			nm::normalize(edge);
			edge *= height;
		}

		nm::Vector3 edge;
		nm::Vector3 base[vcount];
		int baseVerticesCount;
	};

}//