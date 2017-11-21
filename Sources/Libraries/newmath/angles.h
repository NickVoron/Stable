#pragma once

#include "vector.h"
#include "polar3.h"

namespace nm
{	
	float azimuth(const Vector2& point, const Vector2& target);
	float azimuth(const Vector3& point, const Vector3& target);

	float vertical(const Vector3& point, const Vector3& target);

	float degrees(float radians);
	float radians(float degrees);
	Vector3 degrees(const Vector3& radians);
	Vector3 radians(const Vector3& degrees);

	float simplify_angle(float radians);
	float substract_angle(float radians0, float radians1);
	Vector3 simplify_angles(const Vector3& radians);
	Vector3 substract_angles(const Vector3& radians0, const Vector3& radians1);


	Vector3 convertPolar3ToVector3(const Polar3& polar);
	Polar3 convertVector3ToPolar3(const Vector3& vector);
}






