// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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


	inline Vector3 convertPolar3ToVector3(const Polar3& polar)
	{
		float sp, cp, st, ct;
		sp = sinf(polar.phi);
		cp = cosf(polar.phi);
		st = sinf(polar.theta);
		ct = cosf(polar.theta);
		return Vector3(cp * polar.radius * ct, cp * polar.radius * st, polar.radius * sp);
	}

	inline Polar3 convertVector3ToPolar3(const Vector3& vector)
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
			if (z > 0.0f) {
				pitch = 90.0f;
			}
			else {
				pitch = 270.0f;
			}
		}
		else
		{
			yaw = nm::degrees(atan2(y, x));
			if (yaw < 0.0f)
			{
				yaw += 360.0f;
			}

			forward = sqrtf(x * x + y * y);
			pitch = nm::degrees(atan2(z, forward));
			if (pitch < 0.0f) {
				pitch += 360.0f;
			}
		}
		return Polar3(sqrtf(x * x + y * y + z * z), yaw, -pitch);
	}

}








// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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