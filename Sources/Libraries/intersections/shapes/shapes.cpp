// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "shapes.h"

#include "debugDrawing/library.include.h"

namespace intersections
{


	
	
	
	
	
	

	Sphere Sphere::zero()
	{
		Sphere result;
		result.center = Vector3(0, 0, 0);
		result.radius = 0;

		return result;
	}

	Sphere Sphere::one(float x, float y, float z)
	{
		Sphere result;
		result.center = Vector3(x, y, z);
		result.radius = 1;

		return result;
	}


	void Sphere::merge(const Sphere& sphere)
	{
		auto& c1 = center;
		const auto& c2 = sphere.center;

		float& r1 = radius;
		const float& r2 = sphere.radius;

		if (nm::magnitude(c1 - c2) + r1 <= r2)
		{
			c1 = c2;
			r1 = r2;
			return;
		}

		if (nm::magnitude(c2 - c1) + r2 < r1)
		{
			return;
		}

		float r0 = (r1 + r2 + nm::magnitude(c1 - c2)) / 2;
		auto c0 = c1 + (c2 - c1)*(r0 - r1) / nm::magnitude(c2 - c1);

		c1 = c0;
		r1 = r0;
	}

	void Sphere::transform(const State& state, float scale)
	{
		center = state.GetLocalVector(center*scale);
		radius = radius *scale;
	}
	

}




// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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