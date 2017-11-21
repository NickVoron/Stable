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

	

	//DebugDrawing::sphere(leftBot, 0.1);
	//DebugDrawing::sphere(leftTop, 0.1);
	//DebugDrawing::sphere(rightTop, 0.1);
	//DebugDrawing::sphere(rightBot, 0.1);

	//Vector3 minCorner = getminCorner(); Vector3 maxCorner = getMaxCorner();
	
	//Vector3 o0(minCorner.x, minCorner.y, minCorner.z);
	//Vector3 o1(minCorner.x, maxCorner.y, minCorner.z);
	//Vector3 o2(minCorner.x, maxCorner.y, maxCorner.z);
	//Vector3 o3(minCorner.x, minCorner.y, maxCorner.z);

	//DebugDrawing::sphere(o0, 0.1);
	//DebugDrawing::sphere(o1, 0.1);
	//DebugDrawing::sphere(o2, 0.1);
	//DebugDrawing::sphere(o3, 0.1);


	//Quad result(o0, o1, o2, o3);
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

	//Vector3 minCorner = getminCorner(); Vector3 maxCorner = getMaxCorner();

	//Vector3 o0(maxCorner.x, minCorner.y, minCorner.z);
	//Vector3 o1(maxCorner.x, maxCorner.y, minCorner.z);
	//Vector3 o2(maxCorner.x, maxCorner.y, maxCorner.z);
	//Vector3 o3(maxCorner.x, minCorner.y, maxCorner.z);

	//DebugDrawing::sphere(o0, 0.1);
	//DebugDrawing::sphere(o1, 0.1);
	//DebugDrawing::sphere(o2, 0.1);
	//DebugDrawing::sphere(o3, 0.1);
	

	return Quad(leftBot, leftTop, rightTop, rightBot);
}

Quad OBB::quadYn() const //y orientated nagative
{
	Vector3 xDim = axis[0] * extent[0];
	Vector3 yDim = axis[1] * extent[1];
	Vector3 zDim = axis[2] * extent[2];

	Vector3 leftBot = center - xDim - yDim - zDim;
	Vector3 leftTop = center + xDim - yDim - zDim;
	Vector3 rightTop = center + xDim - yDim + zDim;
	Vector3 rightBot = center - xDim - yDim + zDim;

	//DebugDrawing::sphere(leftBot, 0.1);
	//DebugDrawing::sphere(leftTop, 0.1);
	//DebugDrawing::sphere(rightTop, 0.1);
	//DebugDrawing::sphere(rightBot, 0.1);


	return Quad(leftBot, leftTop, rightTop, rightBot);
}

Quad OBB::quadYp() const //y orientated positive
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

Quad OBB::quadZn() const //z orientated negative
{
	Vector3 xDim = axis[0] * extent[0];
	Vector3 yDim = axis[1] * extent[1];
	Vector3 zDim = axis[2] * extent[2];

	Vector3 leftBot = center - xDim - yDim - zDim;
	Vector3 leftTop = center + xDim - yDim - zDim;
	Vector3 rightTop = center + xDim + yDim - zDim;
	Vector3 rightBot = center - xDim + yDim - zDim;

	return Quad(leftBot, leftTop, rightTop, rightBot);

	//Vector3 minCorner = getminCorner(); Vector3 maxCorner = getMaxCorner();
	//return Quad(Vector3(minCorner.x, minCorner.y, minCorner.z), Vector3(maxCorner.x, minCorner.y, minCorner.z), Vector3(maxCorner.x, maxCorner.y, minCorner.z), Vector3(minCorner.x, maxCorner.y, minCorner.z));
}

Quad OBB::quadZp() const //z orientated positive
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