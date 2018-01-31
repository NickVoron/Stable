// Copyright (C) 2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "random.impl.h"
#include "operations.h"
#include "const.h"
#include "vector.operations.h"


namespace nm
{
	static Random rn;

	Vector3 hemisphereCosineRand(const Vector3& v, float cosPow)
	{
		float phi = nm::TwoPI * rn.urnd();
		float cos_phi = cosf(phi);
		float sin_phi = sinf(phi);
		float cos_theta = powf(rn.urnd(), 1.0f / (cosPow + 1.0f));
		float sin_theta = sqrtf(1.0f - cos_theta*cos_theta);

		Vector3 cX, cY;
		basis(v, cX, cY);

		
		Vector3 dir;
		dir = cos_phi*cX + sin_phi*cY;
		dir = sin_theta*dir + cos_theta*v;
		return dir;
	}

	void hemisphereCosineRandAngles(float& phi, float& theta, float cosPow)
	{
		phi = nm::TwoPI * rn.urnd();
		float cos_theta = powf(rn.urnd(), 1.0f / (cosPow + 1.0f));
		theta = acosf(cos_theta);
	}

	Vector3 hemisphereUniformRand(const Vector3 &norm)
	{
		float phi = nm::TwoPI * rn.urnd();
		float cos_phi = cosf(phi);
		float sin_phi = sinf(phi);
		float cos_theta = rn.urnd();
		float sin_theta = sqrtf(1.0f - cos_theta*cos_theta);

		Vector3 cX, cY;
		basis(norm, cX, cY);

		Vector3 dir;
		dir = cos_phi*cX + sin_phi*cY;
		dir = sin_theta*dir + cos_theta*norm;
		return dir;
	}

	void hemisphereUniformRandAngles(float& phi, float& theta)
	{
		phi = nm::TwoPI * rn.urnd();
		theta = nm::PI * rn.urnd();
	}

	void sphereUniformRand(Vector3& v)
	{
		float z = rn.srnd();
		float r = sqrtf(1 - z*z);
		float t = rn.urnd() * nm::TwoPI;

		v.x = cosf(t) * r;
		v.y = sinf(t) * r;
		v.z = z;
	}

	float random_urnd()	{	return rn.urnd();	}
	float random_srnd()	{	return rn.srnd();	}


}




// Copyright (C) 2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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