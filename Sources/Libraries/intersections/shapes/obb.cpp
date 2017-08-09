// Copyright (C) 2015 Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "obb.h"
#include "../utils/utils.h"
#include "debugDrawing/library.include.h"

namespace intersections
{

OBB::OBB(const Vector3& center_, const Vector3 a[3], const float e[3]) : center(center_)
{
	axis[0] = a[0];
	axis[1] = a[1];
	axis[2] = a[2];
	extent[0] = e[0];
	extent[1] = e[1];
	extent[2] = e[2];

	normalizeAxis();
}

OBB::OBB(const Vector3& center_, const Vector3& a0, const Vector3& a1, const Vector3& a2, const float e0, const float e1, const float e2) : center(center_)
{
	axis[0] = a0;
	axis[1] = a1;
	axis[2] = a2;
	extent[0] = e0;
	extent[1] = e1;
	extent[2] = e2;

	normalizeAxis();
}

OBB::OBB(const AABB& aabb)
{
	center = aabb.center();
	axis[0] = Vector3(1, 0, 0);
	axis[1] = Vector3(0, 1, 0);
	axis[2] = Vector3(0, 0, 1);

	Vector3 sz = aabb.size();
	extent[0] = sz.x * 0.5f;
	extent[1] = sz.y * 0.5f;
	extent[2] = sz.z * 0.5f;

	normalizeAxis();
}

Quad OBB::quadXn() const 
{
	Vector3 xDim = axis[0] * extent[0];
	Vector3 yDim = axis[1] * extent[1];
	Vector3 zDim = axis[2] * extent[2];

	Vector3 leftBot  = center - xDim - yDim - zDim;
	Vector3 leftTop  = center - xDim + yDim - zDim;
	Vector3 rightTop = center - xDim + yDim + zDim;
	Vector3 rightBot = center - xDim - yDim + zDim;

	

	
	
	
	

	
	
	
	
	
	

	
	
	
	


	
	Quad result(leftBot, leftTop, rightTop, rightBot);

	return result;
}

Quad OBB::quadXp() const 
{
	Vector3 xDim = axis[0] * extent[0];
	Vector3 yDim = axis[1] * extent[1];
	Vector3 zDim = axis[2] * extent[2];

	Vector3 leftBot = center + xDim - yDim - zDim;
	Vector3 leftTop = center + xDim + yDim - zDim;
	Vector3 rightTop = center + xDim + yDim + zDim;
	Vector3 rightBot = center + xDim - yDim + zDim;

	

	
	
	
	

	
	
	
	
	

	return Quad(leftBot, leftTop, rightTop, rightBot);
}

Quad OBB::quadYn() const 
{
	Vector3 xDim = axis[0] * extent[0];
	Vector3 yDim = axis[1] * extent[1];
	Vector3 zDim = axis[2] * extent[2];

	Vector3 leftBot = center - xDim - yDim - zDim;
	Vector3 leftTop = center + xDim - yDim - zDim;
	Vector3 rightTop = center + xDim - yDim + zDim;
	Vector3 rightBot = center - xDim - yDim + zDim;

	
	
	
	


	return Quad(leftBot, leftTop, rightTop, rightBot);
}

Quad OBB::quadYp() const 
{
	Vector3 xDim = axis[0] * extent[0];
	Vector3 yDim = axis[1] * extent[1];
	Vector3 zDim = axis[2] * extent[2];

	Vector3 leftBot = center - xDim + yDim - zDim;
	Vector3 leftTop = center + xDim + yDim - zDim;
	Vector3 rightTop = center + xDim + yDim + zDim;
	Vector3 rightBot = center - xDim + yDim + zDim;

	return Quad(leftBot, leftTop, rightTop, rightBot);
}

Quad OBB::quadZn() const 
{
	Vector3 xDim = axis[0] * extent[0];
	Vector3 yDim = axis[1] * extent[1];
	Vector3 zDim = axis[2] * extent[2];

	Vector3 leftBot = center - xDim - yDim - zDim;
	Vector3 leftTop = center + xDim - yDim - zDim;
	Vector3 rightTop = center + xDim + yDim - zDim;
	Vector3 rightBot = center - xDim + yDim - zDim;

	return Quad(leftBot, leftTop, rightTop, rightBot);

	
	
}

Quad OBB::quadZp() const 
{
	Vector3 xDim = axis[0] * extent[0];
	Vector3 yDim = axis[1] * extent[1];
	Vector3 zDim = axis[2] * extent[2];

	Vector3 leftBot = center - xDim - yDim + zDim;
	Vector3 leftTop = center + xDim - yDim + zDim;
	Vector3 rightTop = center + xDim + yDim + zDim;
	Vector3 rightBot = center - xDim + yDim + zDim;

	return Quad(leftBot, leftTop, rightTop, rightBot);
}


std::vector<Vector3>  OBB::getCorners() const
{
	std::vector<Vector3> result;
	result.resize(8);

	Quad qXn = quadXn();
	Quad qXp = quadXp();

	for (int i = 0; i < 4; i++)
	{
		result[i] = qXp.vertices[i];
		result[4 + i] = qXn.vertices[i];
	}

	return result;
}

float OBB::boudingSphere()
{
	Vector3 xDim = axis[0] * extent[0];
	Vector3 yDim = axis[1] * extent[1];
	Vector3 zDim = axis[2] * extent[2];

	return (xDim+yDim+zDim).Magnitude();
}

}//



// Copyright (C) 2015 Denis Netakhin <denis.netahin@yandex.ru>
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