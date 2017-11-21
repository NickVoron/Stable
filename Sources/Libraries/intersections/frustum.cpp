#include "frustum.h"
#include "geometry/library.include.h"
#include "algorithm/threePlanes.h"
#include "debugDrawing/library.include.h"


namespace intersections
{

	Frustum::Frustum(){}
	Frustum::Frustum(const Matrix4& m)
	{
		build(m);
	}

	void Frustum::build(const Matrix4& mtx)
	{
		planes[0] = nm::plane(Vector3(mtx(3,0)+mtx(0,0), mtx(3,1)+mtx(0,1), mtx(3,2)+mtx(0,2)), mtx(3,3)+mtx(0,3));
		planes[1] = nm::plane(Vector3(mtx(3,0)-mtx(0,0), mtx(3,1)-mtx(0,1), mtx(3,2)-mtx(0,2)), mtx(3,3)-mtx(0,3));

		planes[2] = nm::plane(Vector3(mtx(3,0)+mtx(1,0), mtx(3,1)+mtx(1,1), mtx(3,2)+mtx(1,2)), mtx(3,3)+mtx(1,3));
		planes[3] = nm::plane(Vector3(mtx(3,0)-mtx(1,0), mtx(3,1)-mtx(1,1), mtx(3,2)-mtx(1,2)), mtx(3,3)-mtx(1,3));

		planes[4] = nm::plane(Vector3(mtx(2,0), mtx(2,1), mtx(2,2)), mtx(2,3));
		planes[5] = nm::plane(Vector3(mtx(3,0)-mtx(2,0), mtx(3,1)-mtx(2,1), mtx(3,2)-mtx(2,2)), mtx(3,3)-mtx(2,3));

		for(int i = 0; i < 6; ++i)	planes[i].normalize();
	}

	bool Frustum::isInside(const Vector3& p) const
	{
		for(int i = 0; i < 6; ++i) 
			if(planes[i].distance(p) > 0) return false;
		return true;
	}


	bool Frustum::aabb(intersections::AABB& aabb)
	{
		const int cornersCount = 8;
		Vector3 corners[cornersCount];

		if (!intersections::test(top(),		faraway(),	left(),		corners[0])) return false;
		if (!intersections::test(top(),		faraway(),	right(),	corners[1])) return false;

		if (!intersections::test(top(),		nigh(),		left(),		corners[2])) return false;
		if (!intersections::test(top(),		nigh(),		right(),	corners[3])) return false;

		if (!intersections::test(bottom(),	faraway(),	left(),		corners[4])) return false;
		if (!intersections::test(bottom(),	faraway(),	right(),	corners[5])) return false;

		if (!intersections::test(bottom(),	nigh(),		left(),		corners[6])) return false;
		if (!intersections::test(bottom(),	nigh(),		right(),	corners[7])) return false;

		Geometry::computeBoundBox(corners, cornersCount, aabb);

		return true;
	}



/*
	Frustum::IntersectionType Frustum::intersect(const Box& box) const
	{
		IntersectionType ret = IT_INSIDE; 
		Vector3 vmin, vmax; 
		Vector3 mins, maxs; 
		box.getAABB(mins, maxs);

		for(int i = 0; i < 6; ++i) 
		{ 
			// X axis 
			if(planes[i].n.x > 0) 
			{ 
				vmin.x = mins.x; 
				vmax.x = maxs.x; 
			} 
			else 
			{ 
				vmin.x = maxs.x; 
				vmax.x = mins.x; 
			} 

			// Y axis 
			if(planes[i].n.y > 0) 
			{ 
				vmin.y = mins.y; 
				vmax.y = maxs.y; 
			} 
			else 
			{ 
				vmin.y = maxs.y; 
				vmax.y = mins.y; 
			} 

			// Z axis 
			if(planes[i].n.z > 0) 
			{ 
				vmin.z = mins.z; 
				vmax.z = maxs.z; 
			} 
			else 
			{ 
				vmin.z = maxs.z; 
				vmax.z = mins.z; 
			} 

			if( planes[i].distToPoint(vmax) < 0 ) return IT_OUTSIDE; 
			if( planes[i].distToPoint(vmin)<= 0 ) ret = IT_INTERSECT; 
		} 

		return ret; 
	}*/



}

