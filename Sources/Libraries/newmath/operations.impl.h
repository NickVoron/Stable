// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "operations.h"
#include "const.h"

#undef F

namespace nm
{

template<class T>
T sqr(T v)
{	
	return v*v;
}


template<class T, class Scalar>
T lerp(const T& v0, const T& v1, Scalar t)
{	
	const Scalar s = 1.0f - t;
	return v0*s + v1*t;
}			

template<class F> 
F sign(F f)
{
	return f > zero<F>::value() ? one<F>::value() : ( f < zero<F>::value() ? -one<F>::value() : zero<F>::value() );
}

template<class F> 
F abs(F f)
{
	return f < zero<F>::value() ? -f : f;
}

template<class F> 
F clamp(F x, F dn, F up)
{
	return ( x >= dn ? ( x <= up ? x : up ) : dn );
}

template<class F> 
F saturate(F f)
{
	return clamp(f, 0.0f, 1.0f);
}

template<class F> 
F degrees(F rad)
{
	return rad * 180.0f / PI;
}

template<class F> 
F radians(F deg)
{
	return deg * PI_180; 
}

template<class F> F normalize_angle(F f)
{
	F res = fmodf( f, TwoPI );
	while ( res >  PI ) res -= TwoPI;
	while ( res < -PI ) res += TwoPI;
	return res;
}

template<class F> 
F round(F f)
{
	return floorf(f + 0.5f);
}

template<class F> 
F trunc(F f)
{
	return floorf(f);
}

template<class F> 
F frac(F f)
{
	return f - trunc(f);
}

template<class F> 
F min(F v1, F v2)
{
	return v1 < v2 ? v1 : v2;
}

template<class F> 
F max(F v1, F v2)
{
	return v1 > v2 ? v1 : v2;
}

template<class F> vector2_t<F> max(const vector2_t<F>& v1, const vector2_t<F>& v2) { return vector2_t<F>(std::max(v1.x, v2.x), std::max(v1.y, v2.y)); }
template<class F> vector2_t<F> min(const vector2_t<F>& v1, const vector2_t<F>& v2) { return vector2_t<F>(std::min(v1.x, v2.x), std::min(v1.y, v2.y)); }

template<class F> vector3_t<F> max(const vector3_t<F>& v1, const vector3_t<F>& v2) { return vector3_t<F>(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z)); }
template<class F> vector3_t<F> min(const vector3_t<F>& v1, const vector3_t<F>& v2) { return vector3_t<F>(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z)); }

template<class F> vector4_t<F> max(const vector4_t<F>& v1, const vector4_t<F>& v2) { return vector4_t<F>(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z), std::max(v1.w, v2.w)); }
template<class F> vector4_t<F> min(const vector4_t<F>& v1, const vector4_t<F>& v2) { return vector4_t<F>(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z), std::min(v1.w, v2.w)); }

inline void transpose(matrix4& m)
{
	matrix4 tm = m;
	m.m[0][0] = tm.m[0][0];
	m.m[0][1] = tm.m[1][0];
	m.m[0][2] = tm.m[2][0];
	m.m[0][3] = tm.m[3][0];

	m.m[1][0] = tm.m[0][1];
	m.m[1][1] = tm.m[1][1];
	m.m[1][2] = tm.m[2][1];
	m.m[1][3] = tm.m[3][1];

	m.m[2][0] = tm.m[0][2];
	m.m[2][1] = tm.m[1][2];
	m.m[2][2] = tm.m[2][2];
	m.m[2][3] = tm.m[3][2];

	m.m[3][0] = tm.m[0][3];
	m.m[3][1] = tm.m[1][3];
	m.m[3][2] = tm.m[2][3];
	m.m[3][3] = tm.m[3][3];
}










template<class Vector3T1, class Vector3T2, class Vector3T3>
inline float area3D(const Vector3T1& v0, const Vector3T2& v1, const Vector3T3& v2)
{
	Vector3T1 cp;
	cross(v2 - v0, v1 - v0, cp);
	return 0.5f * magnitude(cp);
}

template<class Vector2T1, class Vector2T2, class Vector2T3>
inline float area2D(const Vector2T1& a, const Vector2T2& b, const Vector2T3& c)
{
	return 0.5f *((b.x - a.x)*(c.y - a.y) - (c.x - a.x)*(b.y - a.y));
}

template<class Vector2Type1, class Vector2Type2, class Vector2Type3>
float point2ToLineSegmentDistance(const Vector2Type1& p, const Vector2Type2& a, const Vector2Type3& b)
{
	return (b.x - a.x)*(p.y - a.y) - (b.y - a.y)*(p.x - a.x);
}

template<class Vector3Type>
Vector3Type calculateNormal(const Vector3Type& a, const Vector3Type& b, const Vector3Type& c)
{
	Vector3Type ac = c - a;
	Vector3Type ab = b - a;

	Vector3Type n;
	cross(ab, ac, n);
	normalize(n);
	return n;
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