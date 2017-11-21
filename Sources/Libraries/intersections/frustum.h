#pragma once

#include "math/library.include.h"
#include "newmath/library.include.h"
#include "shapes/shapes.h"
#include "shapes/aabb.h"
							 
namespace intersections
{


	//planes[0] left
	//planes[1] right

	//planes[2] top
	//planes[3] bottom

	//planes[4] near
	//planes[5] far
	class Frustum
	{
	public:
		enum IntersectionType	{ IT_OUTSIDE, IT_INTERSECT, IT_INSIDE };

		Frustum();
		Frustum(const Matrix4& m);

		void build(const Matrix4& m);

		bool isInside(const Vector3& p) const;

		//IntersectionType intersect(const Box& box) const;

		template<bool IntersectIsInterest>
		IntersectionType intersect(const Vector3& pos, float radius) const;

		template<bool IntersectIsInterest>
		IntersectionType intersect(const intersections::Sphere& s) const
		{
			return intersect<IntersectIsInterest>(s.center, s.radius);
		}

		int boxInFrustum(const intersections::AABB &b);

		nm::plane planes[6];


		inline nm::plane& left()	{ return planes[0]; }
		inline nm::plane& right()	{ return planes[1]; }

		inline nm::plane& top()		{ return planes[2]; }
		inline nm::plane& bottom()	{ return planes[3]; }

		inline nm::plane& nigh()	{ return planes[4]; }		//near - эти слова задефаненны как макросы minwindef.h
		inline nm::plane& faraway()	{ return planes[5]; }		//far

		bool aabb(intersections::AABB& aabb);					// строит про углам frustum aabb

	};
    
    
    template<>
 	inline Frustum::IntersectionType Frustum::intersect<true>(const Vector3& pos, float radius) const
 	{
 		IntersectionType ret = IT_INSIDE;
 		for(int i = 0; i < 6; ++i)
 		{
 		float fDistance = planes[i].distance(pos);
 			if(fDistance < -radius)	return IT_OUTSIDE;
 			if(fabsf(fDistance) < radius) ret = IT_INTERSECT;
 		}
 		return ret;
 	}
 					 

    template<>
    inline Frustum::IntersectionType Frustum::intersect<false>(const Vector3& pos, float radius) const
    {
        
        float negRadius = -radius;
        return (	( planes[0].distance(pos) < negRadius ) 						 
                ||	( planes[1].distance(pos) < negRadius ) 
                ||	( planes[2].distance(pos) < negRadius ) 
                ||	( planes[3].distance(pos) < negRadius ) 
                ||	( planes[4].distance(pos) < negRadius ) 
                ||	( planes[5].distance(pos) < negRadius )) ? IT_OUTSIDE : IT_INSIDE;
    }

    
}
