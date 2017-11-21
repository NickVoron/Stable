#pragma once
#include "quad.h"


namespace intersections
{
	struct OBB;

	struct AABB
	{
		AABB() { collapse(); }

		nm::Vector3 bmin;
		nm::Vector3 bmax;

		nm::Vector3 center() const 	{ return bmin + size() * 0.5f; }
		nm::Vector3 size() const { return bmax - bmin; }

		void collapse();

		AABB operator+(const AABB& a2);
		void push(const nm::Vector3& point);
		
			
		void combine(const AABB& aabb);
		

		nm::plane planeXn() const { return nm::plane(bmin, bmin + nm::Vector3(0, 1, 0), bmin + nm::Vector3(0, 0, 1)); }
		nm::plane planeXp() const { return nm::plane(bmax, bmax + nm::Vector3(0, 1, 0), bmax + nm::Vector3(0, 0, 1)); }
		nm::plane planeYn() const { return nm::plane(bmin, bmin + nm::Vector3(1, 0, 0), bmin + nm::Vector3(0, 0, 1)); }
		nm::plane planeYp() const { return nm::plane(bmax, bmax + nm::Vector3(1, 0, 0), bmax + nm::Vector3(0, 0, 1)); }
		nm::plane planeZn() const { return nm::plane(bmin, bmin + nm::Vector3(0, 1, 0), bmin + nm::Vector3(1, 0, 0)); }
		nm::plane planeZp() const { return nm::plane(bmax, bmax + nm::Vector3(0, 1, 0), bmax + nm::Vector3(1, 0, 0)); }

		Quad quadXn() const { return Quad(nm::Vector3(bmin.x, bmin.y, bmin.z), nm::Vector3(bmin.x, bmax.y, bmin.z), nm::Vector3(bmin.x, bmax.y, bmax.z), nm::Vector3(bmin.x, bmin.y, bmax.z)); }
		Quad quadXp() const { return Quad(nm::Vector3(bmax.x, bmin.y, bmin.z), nm::Vector3(bmax.x, bmax.y, bmin.z), nm::Vector3(bmax.x, bmax.y, bmax.z), nm::Vector3(bmax.x, bmin.y, bmax.z)); }

		Quad quadYn() const { return Quad(nm::Vector3(bmin.x, bmin.y, bmin.z), nm::Vector3(bmax.x, bmin.y, bmin.z), nm::Vector3(bmax.x, bmin.y, bmax.z), nm::Vector3(bmin.x, bmin.y, bmax.z)); }
		Quad quadYp() const { return Quad(nm::Vector3(bmin.x, bmax.y, bmin.z), nm::Vector3(bmax.x, bmax.y, bmin.z), nm::Vector3(bmax.x, bmax.y, bmax.z), nm::Vector3(bmin.x, bmax.y, bmax.z)); }

		Quad quadZn() const { return Quad(nm::Vector3(bmin.x, bmin.y, bmin.z), nm::Vector3(bmax.x, bmin.y, bmin.z), nm::Vector3(bmax.x, bmax.y, bmin.z), nm::Vector3(bmin.x, bmax.y, bmin.z)); }
		Quad quadZp() const { return Quad(nm::Vector3(bmin.x, bmin.y, bmax.z), nm::Vector3(bmax.x, bmin.y, bmax.z), nm::Vector3(bmax.x, bmax.y, bmax.z), nm::Vector3(bmin.x, bmax.y, bmax.z)); }

		bool operator==(const AABB& aabb) const;
		void transform(const State& state, float scale);

	};

}//