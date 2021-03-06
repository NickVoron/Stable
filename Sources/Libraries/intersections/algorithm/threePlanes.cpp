// Copyright (C) 2016-2017 Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "threePlanes.h"
#include "math/library.include.h"

namespace intersections
{

bool test(const nm::plane& plane0, const nm::plane& plane1, const nm::plane& plane2, Vector3& result)
{

	Matrix3 d;
	d(0, 0) = plane0.n.x; d(0, 1) = plane0.n.y; d(0, 2) = plane0.n.z;
	d(1, 0) = plane1.n.x; d(1, 1) = plane1.n.y; d(1, 2) = plane1.n.z;
	d(2, 0) = plane2.n.x; d(2, 1) = plane2.n.y; d(2, 2) = plane2.n.z;

	float det = d.GetDeterminant();

	if (det!=0)
	{
		Matrix3 d1;
		d1(0, 0) = -1 * plane0.d; d1(0, 1) = plane0.n.y; d1(0, 2) = plane0.n.z;
		d1(1, 0) = -1 * plane1.d; d1(1, 1) = plane1.n.y; d1(1, 2) = plane1.n.z;
		d1(2, 0) = -1 * plane2.d; d1(2, 1) = plane2.n.y; d1(2, 2) = plane2.n.z;

		Matrix3 d2;
		d2(0, 0) = plane0.n.x; d2(0, 1) = -1 * plane0.d; d2(0, 2) = plane0.n.z;
		d2(1, 0) = plane1.n.x; d2(1, 1) = -1 * plane1.d; d2(1, 2) = plane1.n.z;
		d2(2, 0) = plane2.n.x; d2(2, 1) = -1 * plane2.d; d2(2, 2) = plane2.n.z;


		Matrix3 d3;
		d3(0, 0) = plane0.n.x; d3(0, 1) = plane0.n.y; d3(0, 2) = -1 * plane0.d;
		d3(1, 0) = plane1.n.x; d3(1, 1) = plane1.n.y; d3(1, 2) = -1 * plane1.d;
		d3(2, 0) = plane2.n.x; d3(2, 1) = plane2.n.y; d3(2, 2) = -1 * plane2.d;



		float det1 = d1.GetDeterminant();
		float det2 = d2.GetDeterminant();
		float det3 = d3.GetDeterminant();

		result.x = det1 / det;
		result.y = det2 / det;
		result.z = det3 / det;

		return true;
	}

	return false;

}
}//



// Copyright (C) 2016-2017 Denis Netakhin <denis.netahin@yandex.ru>
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