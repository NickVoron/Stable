// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "quaternion.h"
#include "const.h"

namespace nm
{
	void hpb(const Quaternion &qu, Vector3 &result)
	{
		float x = qu.v.x;
		float y = qu.v.y;
		float z = qu.v.z;
		float w = qu.w;
		float& h = result.x;
		float& p = result.y;
		float& b = result.z;

		float test = x * y + z * w;
		if( !( test > 0.499f ||  test < -0.499f) )
		{
			float tx = 2.0f * x; 
			float ty = 2.0f * y;
			float o = 1.0f - 2.0f*z*z;
			h = atan2f(ty * w - tx * z, o - ty*y);
			p = -atan2f(tx * w - ty * z, o - tx*x);
			b = -asinf(2 * test);
		}
		else
		{
			
			b = 0;
			h = 2 * atan2f(x, w);
			p = PI_2;

			if (test < -0.499f) 
			{ 
				h = -h;
				p = -p;
			}
		}
	}

	float heading(const Quaternion& q)
	{
		float x = q.v.x;
		float y = q.v.y;
		float z = q.v.z;
		float w = q.w;

		float test = x * y + z * w;

		if( !( test > 0.499f ||  test < -0.499f) )
		{
			return atan2f(2.0f*y*w - 2.0f*x*z, 1.0f-2.0f*y*y - 2.0f*z*z);
		}
		else
		{
			float r = 2.0f * atan2f(x, w);
			return ( test > 0.499f ) ? r : -r;
		}
	}

	float pitch(const Quaternion& q)
	{
		float x = q.v.x;
		float y = q.v.y;
		float z = q.v.z;
		float w = q.w;

		float test = x * y + z * w;
		if( !( test > 0.499f ||  test < -0.499f) )
		{
			return -atan2f(2.0f*x*w - 2.0f*y*z, 1.0f - 2.0f*x*x - 2.0f*z*z);
		}
		else
		{
			return ( test > 0.499f ) ? PI_2 : -PI_2;
		}
	}

	float bank(const Quaternion& q)
	{
		float x = q.v.x;
		float y = q.v.y;
		float z = q.v.z;
		float w = q.w;

		float test = x * y + z * w;
		return !( test > 0.499f ||  test < -0.499f) ? asinf(2.0f * test) : 0.0f;
	}
}




// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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