// Copyright (C) 2015-2016 Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



// Copyright (C) 2015-2016 Denis Netakhin <denis.netahin@yandex.ru>
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