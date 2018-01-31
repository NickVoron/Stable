// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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