// Copyright (C) 2012-2016 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "vector.h"
#include "vector.operations.h"

namespace nm
{
	class plane
	{
	public:
		Vector3 n;
		float d;

		plane(){}

		
		plane(const Vector3& pn, float pd) : n(pn), d(pd) {}

		template<class Vec3T1, class Vec3T2, class Vec3T3>
		plane(const Vec3T1& p0, const Vec3T2& p1, const Vec3T3& p2)
		{
			build(p0, p1, p2);
		}

		template<class Vec3T1, class Vec3T2, class Vec3T3>
		void build(const Vec3T1& p0, const Vec3T2& p1, const Vec3T3& p2) 
		{	
			cross((p1 - p0), (p2 - p0), n);
			nm::normalize(n);
			d = -dot(p0, n);
		}

		
		template<class Vec3Type>
		float distance(const Vec3Type& v) const 
		{	
			return dot(n, v) + d;	
		}

		
		template<class Vec3Type1, class Vec3Type2, class Vec3Type3>
		bool intersection(const Vec3Type1& org, const Vec3Type2& dir, Vec3Type3* pos = 0, float* alpha = 0) const
		{	
			float dc = dot(org, n) + d;
			float dn = dot(dir, n);

			if(fabsf(dn) < 0.000001f)
				return false;
			
			float t = -dc / dn;

			if(alpha)
			{
				*alpha = t;
			}

 			if(t < 0.0f)
 				return false;

			if(pos)						
			{
				pos->x = org.x + t * dir.x;
				pos->y = org.y + t * dir.y;
				pos->z = org.z + t * dir.z;
			}

			return true;
		}

		template<class Vec3Type>
		Vec3Type projection(const Vec3Type& org) const 
		{	
			float dc = dot(org, n) + d;
			float dn = dot(-n, n);

			
			

			float t = -dc / dn;

			Vec3Type pos;
			
			pos.x = org.x + t * -n.x;
			pos.y = org.y + t * -n.y;
			pos.z = org.z + t * -n.z;
			
			return pos;
		}	

		template<class Vec3>
		plane parallel(const Vec3& point) const
		{
			plane p;
			p.n = n;
			p.d = -dot(n, point);

			return p;
		}

		bool isParallel(const plane& p) const
		{
			return fabsf(fabsf(dot(n, p.n)) - 1.0f) < 0.00001f;	
		}


		void normalize()
		{
			float l = 1.0f / magnitude(n);
			n *= l;
			d *= l;
		}	

		Vector3 v() const
		{
			return n*d;
		}
	};

}



// Copyright (C) 2012-2016 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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