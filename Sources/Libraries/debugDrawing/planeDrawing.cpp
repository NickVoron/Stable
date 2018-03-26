// Copyright (C) 2016-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//


#include "planeDrawing.h"

namespace DebugDrawing
{

	void plane(const nm::plane& plane, const Vector3& basis)
	{
		auto basePoint = plane.projection(basis);
		

		Vector3 planeVector = plane.n;
		Vector3 randomVector;
		if (!Vector3::zAxis.Collinear(planeVector))
		{
			randomVector = Vector3::zAxis;
		}
		else
		{
			randomVector = Vector3::xAxis;
		}

		
		
		Vector3 base1 = planeVector.SCross(randomVector);
		base1.Normalize();

		Vector3 base2 = planeVector.SCross(base1);
		base2.Normalize();

		

		
		
		


		for (float i = -50; i<= 50; i+=10 )
		{
			Vector3 p1 = basePoint + base1*i - base2 * 50.0f;
			Vector3 p2 = basePoint + base1*i + base2 * 50.0f;
            #ifdef USE_WINDOWS
			line(p1, p2);
            #endif

		}

		for (float i = -50; i <= 50; i+=10)
		{
			Vector3 p1 = basePoint + base2*i - base1 * 50.0f;
			Vector3 p2 = basePoint + base2*i + base1 * 50.0f;
            #ifdef USE_WINDOWS
			line(p1, p2);
            #endif

		}



	}

}




// Copyright (C) 2016-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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