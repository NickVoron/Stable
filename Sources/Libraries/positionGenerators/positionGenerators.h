#pragma once

#include "math/library.include.h"

#include <vector>

namespace Positions
{
	std::vector<Vector3> circleXZ(std::size_t count, Vector3 center, float circleRadius);
	std::vector<Vector3> circleYZ(std::size_t count, Vector3 center, float circleRadius);
	std::vector<Vector3> circleXY(std::size_t count, Vector3 center, float circleRadius);

	std::vector<Vector3> regularXZ(Vector2 minCorner, Vector2 maxCorner, float y, Vector2 step);
	std::vector<Vector3> regularYZ(Vector2 minCorner, Vector2 maxCorner, float x, Vector2 step);
	std::vector<Vector3> regularXY(Vector2 minCorner, Vector2 maxCorner, float z, Vector2 step);
	std::vector<Vector3> regularXYZ(Vector3 minCorner, Vector3 maxCorner, Vector3 step);
}