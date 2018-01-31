// Copyright (C) 2015 Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "sphere2obbMk2.h"


namespace intersections
{


	void clamp(Vector3& v, const Vector3& max)
	{
		if(v.MagnitudeSquared() > max.MagnitudeSquared())
		{
			v.Normalize();
			v*= max.Magnitude();
		}
	}

	void nearestPointMk2(const Vector3& N, const OBB& obb, Vector3& point)
	{
		Vector3 result(0,0,0);

		const Vector3 NC = N-obb.center;

		for (int i=0; i<3; i++)
		{
			Vector3 normalizedAxis = obb.axis[i];
			normalizedAxis.Normalize();
			
			Vector3 sizedAxis = normalizedAxis * obb.extent[i];

			float projSize = nm::dot(sizedAxis, NC)/sizedAxis.Magnitude(); 
			
			Vector3 tmp = normalizedAxis*projSize;
			clamp(tmp, sizedAxis);

			result += tmp;
		}



		point = result+obb.center;


	}

}



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