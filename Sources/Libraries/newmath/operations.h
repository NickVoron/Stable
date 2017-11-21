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


//template<class F> void swap(F& v1, F& v2);

inline int ring_clamp(int val, int maxVal)
{
	if(val > maxVal)
	{
		return val % maxVal - 1;
	}

	return val;
}

//p,q -> cross(p, q) = n (n - должен быть нормализован!)
void basis(const Vector3& n, Vector3& p, Vector3& q);

}

