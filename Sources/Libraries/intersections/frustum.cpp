// Copyright (C) 2012-2016 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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