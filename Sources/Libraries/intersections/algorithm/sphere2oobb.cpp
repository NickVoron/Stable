#include "sphere2oobb.h"
#include "../debug.h"
#include "debugDrawing/library.include.h"
#include "ray2shapes.h"

namespace intersections
{

bool nearestPoint(const Sphere& shpere, const OBB& box, Vector3& point)
{
	
	Quad quads[6] =
	{
		box.quadXp(),
		box.quadXn(),
		box.quadYp(),
		box.quadYn(),
		box.quadZp(),
		box.quadZn(),
	};

	float minAlpha = FLT_MAX;
	Vector3 res;
	int minDistIdx = -1;

	for (int i = 0; i < 6; ++i)
	{
		Vector3 pos;
		if (haveNormalProjection(shpere, quads[i], pos))
		{
			float alpha = nm::magnitudeSq(pos - shpere.center);
			if (minAlpha > alpha)
			{
				res = pos;
				minAlpha = alpha;
				minDistIdx = i;
			}
		}
	}

	if (minDistIdx >= 0) // ближайшая точка на одной из плоскостей куба
	{
		point = res;
		return true;
	}

	//значит на ребре или угле 
	minAlpha = FLT_MAX;
	Segment3 ribs[12];

	Quad quadXn = box.quadXn();
	Quad quadXp = box.quadXp();

	ribs[0] = { quadXn.vertices[0], quadXn.vertices[1]};
	ribs[1] = { quadXn.vertices[1], quadXn.vertices[2]};
	ribs[2] = { quadXn.vertices[2], quadXn.vertices[3]};
	ribs[3] = { quadXn.vertices[3], quadXn.vertices[0]};

	ribs[4] = { quadXp.vertices[0], quadXn.vertices[0] };
	ribs[5] = { quadXp.vertices[1], quadXn.vertices[1] };
	ribs[6] = { quadXp.vertices[2], quadXn.vertices[2] };
	ribs[7] = { quadXp.vertices[3], quadXn.vertices[3] };

	ribs[8]	 = { quadXp.vertices[0], quadXp.vertices[1] };
	ribs[9]  = { quadXp.vertices[1], quadXp.vertices[2] };
	ribs[10] = { quadXp.vertices[2], quadXp.vertices[3] };
	ribs[11] = { quadXp.vertices[3], quadXp.vertices[0] };

	minAlpha = FLT_MAX;
	minDistIdx = -1;
	for (int i=0; i< 12; i++)
	{
		Vector3 pos;
		Segment3& rib = ribs[i];
		nearestPoint(shpere.center, rib, pos);
		float alpha = nm::magnitudeSq(pos - shpere.center);
		if (minAlpha > alpha)
		{
			res = pos;
			minAlpha = alpha;
			minDistIdx = i;
		}
	}

	if (minDistIdx >= 0) // пересечение на гране
	{
		point = res;
		return true;
	}


	return false;
}


bool haveNormalProjection(const Sphere& shpere, const Quad& quad, Vector3& point)
{
	Triangle trg;
	trg.vertices[0] = quad.vertices[0];
	trg.vertices[1] = quad.vertices[1];
	trg.vertices[2] = quad.vertices[2];
	
	const Vector3& center = shpere.center;

	Vector3 trianglNormal;
	nm::cross((trg.vertices[0]-trg.vertices[1]), (trg.vertices[2]-trg.vertices[1]), trianglNormal);
	
	Ray normalRay(center, trianglNormal);
	Ray backNormalRay(center, -trianglNormal);

	Vector3 centre2vetr = quad.vertices[0] - center;
	float normalDot = nm::dot(center, trianglNormal);

	bool res = false;
	if (normalDot<0)
	{
		res = test(normalRay, trg, point);
	}
	else
	{
		res = test(backNormalRay, trg, point);
	}

	//		DebugDrawing::triangle(trg);
	if (!res)
	{
		trg.vertices[0] = quad.vertices[0];
		trg.vertices[1] = quad.vertices[2];
		trg.vertices[2] = quad.vertices[3];
		// 			DebugDrawing::triangle(trg);

		if (normalDot < 0)
		{
			res = test(normalRay, trg, point);
		}
		else
		{
			res = test(backNormalRay, trg, point);
		}
	}

	return res;
}

void nearestPoint(const Vector3& M, const Segment3& seg, Vector3& point)
{
	//debug::draw(seg);

	//DebugDrawing::line(seg.p0, M);
	//DebugDrawing::line(seg.p1, M);

	Vector3 A = seg.p0;
	Vector3 B = seg.p1;

	Vector3 AB = B - A;
	Vector3 AM = M - A;

	float s = nm::dot(AM, AB) / AB.Magnitude();

	float k = s / AB.Magnitude();


	Vector3 AN = AB * k;
	point = A + AN;

	
	Vector3 centr2p0 = seg.p0 - M;
	Vector3 centr2p1 = seg.p1 - M;

	Vector3 projectVector = point - M;

	
	if (k>1)  
	{
		point = seg.p1;
	}

	if(k < 0)
	{
		point = seg.p0;
	}
	/*DebugDrawing::color(Vector3(1,1,0));
	DebugDrawing::sphere(point, 0.1);
	*/
}

}//