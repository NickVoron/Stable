#pragma once
#include "../shapes/shapes.h"
#include "../shapes/obb.h"


namespace intersections
{

	bool nearestPoint(const Sphere& shpere, const OBB& obb, Vector3& point);

	bool haveNormalProjection(const Sphere& shpere, const Quad& quad, Vector3& point);

	void nearestPoint(const Vector3& M, const Segment3& seg, Vector3& point);	// ближайшая точка на сегменте

}