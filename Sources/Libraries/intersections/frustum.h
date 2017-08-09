// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "math/library.include.h"
#include "newmath/library.include.h"
#include "shapes/shapes.h"
#include "shapes/aabb.h"
							 
namespace intersections
{


	
	

	
	

	
	
	class Frustum
	{
	public:
		enum IntersectionType	{ IT_OUTSIDE, IT_INTERSECT, IT_INSIDE };

		Frustum();
		Frustum(const Matrix4& m);

		void build(const Matrix4& m);

		bool isInside(const Vector3& p) const;

		

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

		inline nm::plane& nigh()	{ return planes[4]; }		
		inline nm::plane& faraway()	{ return planes[5]; }		

		bool aabb(intersections::AABB& aabb);					

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




// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.