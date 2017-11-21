#include "angles.h"
#include "vector.operations.h"
#include "const.h"

namespace nm
{
	float azimuth(float xp, float xt, float yp, float yt)
	{
		return !((xp == xt) && (yp == yt)) ? atan2f((xt - xp), (yt - yp)) : 0.0f;
	}

	float azimuth(const Vector2& point, const Vector2& target)	{ return azimuth(point.x, target.x, point.y, target.y); }
	float azimuth(const Vector3& point, const Vector3& target)	{ return azimuth(point.x, target.x, point.z, target.z);	}

	float vertical(const Vector3& point, const Vector3& target)
	{
		return !((point.y == target.y) && (point.z == target.z)) ?
			-atan2f(target.y - point.y, magnitude(Vector2(target.x - point.x, target.z - point.z))) :
			0.0f;
	}

	constexpr float av = 180.0f / nm::PI;
	constexpr float rv = nm::PI / 180.0f;

	float degrees(float a)	{ return a * av;	}
	float radians(float r)	{ return r * rv;	}

	Vector3 degrees(const Vector3& v)	{	return Vector3(degrees(v.x), degrees(v.y), degrees(v.z));	}
	Vector3 radians(const Vector3& v)	{	return Vector3(radians(v.x), radians(v.y), radians(v.z));	}

	float simplify_angle(float r)
	{
		float res = fmodf(r, nm::TwoPI);
		while (res > nm::PI) res -= nm::TwoPI;
		while (res < -nm::PI) res += nm::TwoPI;
		return res;
	}

	float substract_angle(float radians0, float radians1)
	{
		return simplify_angle(radians0 - radians1);
	}

	Vector3 simplify_angles(const Vector3& radians) 
	{ 
		return Vector3(simplify_angle(radians.x), simplify_angle(radians.y), simplify_angle(radians.z));
	}

	Vector3 substract_angles(const Vector3& x, const Vector3& y)
	{
		return Vector3(substract_angle(x.x, y.x), substract_angle(x.y, y.y), substract_angle(x.z, y.z));
	}


	Vector3 convertPolar3ToVector3(const Polar3& polar)
	{
		float sp, cp, st, ct;
		sp = sinf(polar.phi);
		cp = cosf(polar.phi);
		st = sinf(polar.theta);
		ct = cosf(polar.theta);
		return Vector3(cp * polar.radius * ct, cp * polar.radius * st, polar.radius * sp);
	}

	Polar3 convertVector3ToPolar3(const Vector3& vector)
	{
		float forward;
		float yaw;
		float pitch;

		float x = vector.x;
		float y = vector.y;
		float z = vector.z;

		if ((x == 0.0f) && (y == 0.0f))
		{
			yaw = 0.0f;
			if (z > 0.0f) 
			{
				pitch = nm::PI_2;
			}
			else 
			{
				pitch = nm::PI_2 * 3.0f;
			}
		}
		else
		{
			yaw = atan2(y, x);
			if (yaw < 0.0f)
			{
				yaw += nm::TwoPI;
			}

			forward = sqrtf(x * x + y * y);
			pitch = atan2(z, forward);
			if (pitch < 0.0f) 
			{
				pitch += nm::TwoPI;
			}
		}
		return Polar3(sqrtf(x * x + y * y + z * z), yaw, -pitch);
	}

}




