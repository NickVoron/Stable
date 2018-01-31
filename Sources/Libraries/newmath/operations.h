// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "libcfg.h"
#include "vector.h"
#include "matrix.h"

namespace nm
{

template<class T, class Scalar> T lerp(const T& v0, const T& v1, Scalar t);

template<class F> F sqr(F v);

template<class F> F sign(F f);
template<class F> F abs(F f);

template<class F> F clamp(F f, F minf, F maxf);
template<class F> F saturate(F f);

template<class F> F degrees(F f);
template<class F> F radians(F f);
template<class F> F normalize_angle(F f);

template<class F> F round(F f);
template<class F> F trunc(F f);
template<class F> F frac(F f);

template<class F> F min(F v1, F v2);
template<class F> F max(F v1, F v2);

template<class F> vector2_t<F> max(const vector2_t<F>& v1, const vector2_t<F>& v2);
template<class F> vector2_t<F> min(const vector2_t<F>& v1, const vector2_t<F>& v2);

template<class F> vector3_t<F> max(const vector3_t<F>& v1, const vector3_t<F>& v2);
template<class F> vector3_t<F> min(const vector3_t<F>& v1, const vector3_t<F>& v2);

template<class F> vector4_t<F> max(const vector4_t<F>& v1, const vector4_t<F>& v2);
template<class F> vector4_t<F> min(const vector4_t<F>& v1, const vector4_t<F>& v2);




inline int ring_clamp(int val, int maxVal)
{
	if(val > maxVal)
	{
		return val % maxVal - 1;
	}

	return val;
}


void basis(const Vector3& n, Vector3& p, Vector3& q);

}





// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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