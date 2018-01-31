// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "positionGenerators.h"
#include "newmath/library.include.h"

namespace Positions
{
	std::vector<Vector3> regularXZ(Vector2 minv, Vector2 maxv, float y, Vector2 step) { return regularXYZ(Vector3(minv.x, y, minv.y), Vector3(maxv.x, y, maxv.y), Vector3(step.x, 0, step.y)); }
	std::vector<Vector3> regularYZ(Vector2 minv, Vector2 maxv, float x, Vector2 step) { return regularXYZ(Vector3(x, minv.x, minv.y), Vector3(x, maxv.x, maxv.y), Vector3(0, step.x, step.y)); }
	std::vector<Vector3> regularXY(Vector2 minv, Vector2 maxv, float z, Vector2 step) { return regularXYZ(Vector3(minv.x, minv.y, z), Vector3(maxv.x, maxv.y, z), Vector3(step.x, step.y, 0)); }
	std::vector<Vector3> regularXYZ(Vector3 minv, Vector3 maxv, Vector3 step)
	{
		Vector3 range = maxv - minv;
		const std::size_t xx = (fabsf(step.x) > 0.0f) ? static_cast<std::size_t>(nm::round(range.x / step.x)) : 0;
		const std::size_t yy = (fabsf(step.y) > 0.0f) ? static_cast<std::size_t>(nm::round(range.y / step.y)) : 0;
		const std::size_t zz = (fabsf(step.z) > 0.0f) ? static_cast<std::size_t>(nm::round(range.z / step.z)) : 0;

		std::vector<Vector3> result;
		for (std::size_t x = 0; x <= xx; ++x)
		{
			for (std::size_t y = 0; y <= yy; ++y)
			{
				for (std::size_t z = 0; z <= zz; ++z)
				{
					result.push_back(minv + Vector3(step.x * x, step.y * y, step.z * z));
				}
			}
		}

		return result;
	}

	
	std::vector<Vector3> circle(std::size_t count, Vector3 center, float circleRadius, decltype(&Vector3::x) v0, decltype(&Vector3::x) v1, decltype(&Vector3::x) cv)
	{
		std::vector<Vector3> result(count);
		float segmentAngle = nm::TwoPI / count;
		for (std::size_t i = 0; i < count; ++i)
		{
			auto& pos = result[i];

			float a = i * segmentAngle;
						
			pos.*v0 = sinf(a) * circleRadius + center.*v0;
			pos.*v1 = cosf(a) * circleRadius + center.*v1;
			pos.*cv = center.*cv;
		}

		return result;
	}

	std::vector<Vector3> circleXZ(std::size_t count, Vector3 center, float circleRadius)	{ return circle(count, center, circleRadius, &Vector3::x, &Vector3::z, &Vector3::y); }
	std::vector<Vector3> circleXY(std::size_t count, Vector3 center, float circleRadius)	{ return circle(count, center, circleRadius, &Vector3::x, &Vector3::y, &Vector3::z); }
	std::vector<Vector3> circleYZ(std::size_t count, Vector3 center, float circleRadius)	{ return circle(count, center, circleRadius, &Vector3::y, &Vector3::z, &Vector3::x); }
}




// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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