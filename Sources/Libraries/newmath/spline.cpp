// Copyright (C) 2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "spline.h"
#include "vector.operations.h"

namespace nm
{
	nm::Vector3 spline::bezier3(const nm::Vector3& p1, const nm::Vector3& p2, const nm::Vector3& p3, float mu)
	{
		return (1 - mu)*(1 - mu)*p1 + 2 * mu*(1 - mu)*p2 + mu * mu*p3;
	}

	nm::Vector3 spline::bezier4(const nm::Vector3& p1, const nm::Vector3& p2, const nm::Vector3& p3, const nm::Vector3& p4, float mu)
	{
		float mu1 = 1 - mu;
		float mu13 = mu1 * mu1 * mu1;
		float mu3 = mu * mu * mu;

		nm::Vector3	res;

		res.x = mu13*p1.x + 3 * mu*mu1*mu1*p2.x + 3 * mu*mu*mu1*p3.x + mu3*p4.x;
		res.y = mu13*p1.y + 3 * mu*mu1*mu1*p2.y + 3 * mu*mu*mu1*p3.y + mu3*p4.y;
		res.z = mu13*p1.z + 3 * mu*mu1*mu1*p2.z + 3 * mu*mu*mu1*p3.z + mu3*p4.z;

		return res;
	}

	nm::Vector3 spline::bezier(nm::Vector3 *p, int n, float mu)
	{
		int k, kn, nn, nkn;
		float blend, muk, munk;
		nm::Vector3 b(0.0f, 0.0f, 0.0f);

		muk = 1;
		munk = pow(1.0f - mu, n);

		for (k = 0; k <= n; k++) {
			nn = n;
			kn = k;
			nkn = n - k;
			blend = muk * munk;
			muk *= mu;
			if (fabsf(1 - mu) > 0.00001f)
				munk /= (1 - mu);
			while (nn >= 1) {
				blend *= nn;
				nn--;
				if (kn > 1) {
					blend /= (float) kn;
					kn--;
				}
				if (nkn > 1) {
					blend /= (float) nkn;
					nkn--;
				}
			}
			b.x += p[k].x * blend;
			b.y += p[k].y * blend;
			b.z += p[k].z * blend;
		}

		return b;
	}
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