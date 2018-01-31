// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "vector.h"
#include "type_traits.h"

namespace nm
{




template<typename scalar>
NM_INLINE vector2_t<scalar>::vector2_t()
{

}

template<typename scalar>
NM_INLINE vector2_t<scalar>::vector2_t(scalar _x, scalar _y)
:x(_x), y(_y)
{

}

template<typename scalar>
template<typename vec_t>
NM_INLINE vector2_t<scalar>::vector2_t(const vec_t& v)
:x(v.x), y(v.y)
{
	
}

template<typename scalar>
template<typename vec_t>
NM_INLINE typename vector2_t<scalar>::self_t& vector2_t<scalar>::operator=(const vec_t& v)
{
	

	x = v.x;
	y = v.y;
	return *this;
}

template<typename scalar>
NM_INLINE typename vector2_t<scalar>::self_t vector2_t<scalar>::operator-() const
{
	return self_t(-x, -y);
}

template<typename scalar>
template<typename vec_t>
NM_INLINE void vector2_t<scalar>::operator+=(const vec_t& v)
{
	

	x += v.x;
	y += v.y;
}

template<typename scalar>
template<typename vec_t>
NM_INLINE void vector2_t<scalar>::operator-=(const vec_t& v)
{
	

	x -= v.x;
	y -= v.y;
}

template<typename scalar>
template<typename vec_t>
NM_INLINE void vector2_t<scalar>::operator*=(const vec_t& v)
{
	

	x *= v.x;
	y *= v.y;
}

template<typename scalar>
template<typename vec_t>
NM_INLINE void vector2_t<scalar>::operator/=(const vec_t& v)
{
	

	x /= v.x;
	y /= v.y;
}

template<typename scalar>
template<typename vec_t>
NM_INLINE  vec_t vector2_t<scalar>::operator+(const vec_t& v) const
{
	return vec_t(x + v.x, y + v.y);
}

template<typename scalar>
template<typename vec_t>
NM_INLINE  vec_t vector2_t<scalar>::operator-(const vec_t& v) const
{
	return vec_t(x - v.x, y - v.y);
}


template<typename scalar>
NM_INLINE void vector2_t<scalar>::operator*=(scalar s)
{
	x *= s;
	y *= s;
}

template<typename scalar>
NM_INLINE void vector2_t<scalar>::operator/=(scalar s)
{
	float is = 1.0f / s;
	x *= is;
	y *= is;
}

template<typename scalar>
template<class T>
NM_INLINE vector2_t<scalar>::operator T() const
{ 
	
	T res;
	res.x = x;
	res.y = y;
	return res;
}








template<typename scalar>
NM_INLINE vector3_t<scalar>::vector3_t()
{

}

template<typename scalar>
NM_INLINE vector3_t<scalar>::vector3_t(scalar _x, scalar _y, scalar _z)
:x(_x), y(_y), z(_z)
{

}

template<typename scalar>
template<typename vec_t>
NM_INLINE vector3_t<scalar>::vector3_t(const vec_t& v)
:x(v.x), y(v.y), z(v.z)
{
	
}

template<typename scalar>
template<typename vec_t>
NM_INLINE typename vector3_t<scalar>::self_t& vector3_t<scalar>::operator=(const vec_t& v)
{
	

	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

template<typename scalar>
NM_INLINE typename vector3_t<scalar>::self_t vector3_t<scalar>::operator-() const
{
	return self_t(-x, -y, -z);
}

template<typename scalar>
template<typename vec_t>
NM_INLINE void vector3_t<scalar>::operator+=(const vec_t& v)
{
	

	x += v.x;
	y += v.y;
	z += v.z;
}

template<typename scalar>
template<typename vec_t>
NM_INLINE void vector3_t<scalar>::operator-=(const vec_t& v)
{
	

	x -= v.x;
	y -= v.y;
	z -= v.z;
}

template<typename scalar>
template<typename vec_t>
NM_INLINE void vector3_t<scalar>::operator*=(const vec_t& v)
{
	

	x *= v.x;
	y *= v.y;
	z *= v.z;
}

template<typename scalar>
template<typename vec_t>
NM_INLINE void vector3_t<scalar>::operator/=(const vec_t& v)
{
	

	x /= v.x;
	y /= v.y;
	z /= v.z;
}

template<typename scalar>
NM_INLINE void vector3_t<scalar>::operator*=(scalar s)
{
	x *= s;
	y *= s;
	z *= s;
}

template<typename scalar>
NM_INLINE void vector3_t<scalar>::operator/=(scalar s)
{
	float is = 1.0f / s;
	x *= is;
	y *= is;
	z *= is;
}


template<typename scalar>
template<typename vec_t>
NM_INLINE vec_t vector3_t<scalar>::operator+(const vec_t& v) const
{
	
	return vec_t(x+v.x, y + v.y, z + v.z);
}

template<typename scalar>
template<typename vec_t>
NM_INLINE vec_t vector3_t<scalar>::operator-(const vec_t& v) const
{
	

	return vec_t(x - v.x, y - v.y, z - v.z);
}





















template<typename scalar>
NM_INLINE vector4_t<scalar>::vector4_t()
{

}

template<typename scalar>
NM_INLINE vector4_t<scalar>::vector4_t(scalar _x, scalar _y, scalar _z, scalar _w)
:x(_x), y(_y), z(_z), w(_w)
{

}

template<typename scalar>
template<typename vec_t>
NM_INLINE vector4_t<scalar>::vector4_t(const vec_t& v)
:x(v.x), y(v.y), z(v.z), w(v.w)
{
	
}

template<typename scalar>
template<typename vec_t>
NM_INLINE typename vector4_t<scalar>::self_t& vector4_t<scalar>::operator=(const vec_t& v)
{
	

	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}

template<typename scalar>
NM_INLINE typename vector4_t<scalar>::self_t vector4_t<scalar>::operator-() const
{
	return self_t(-x, -y, -z, -w);
}

template<typename scalar>
template<typename vec_t>
NM_INLINE void vector4_t<scalar>::operator+=(const vec_t& v)
{
	

	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

template<typename scalar>
template<typename vec_t>
NM_INLINE void vector4_t<scalar>::operator-=(const vec_t& v)
{
	

	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

template<typename scalar>
template<typename vec_t>
NM_INLINE void vector4_t<scalar>::operator*=(const vec_t& v)
{
	

	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
}

template<typename scalar>
template<typename vec_t>
NM_INLINE void vector4_t<scalar>::operator/=(const vec_t& v)
{
	

	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
}

template<typename scalar>
NM_INLINE void vector4_t<scalar>::operator*=(scalar s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
}

template<typename scalar>
NM_INLINE void vector4_t<scalar>::operator/=(scalar s)
{
	float is = 1.0f / s;
	x *= is;
	y *= is;
	z *= is;
	w *= is;
}

template<typename scalar>
template<class T>
NM_INLINE vector4_t<scalar>::operator T() const
{
	
	T res;
	res.x = x;
	res.y = y;
	res.z = z;
	res.w = w;
	return res;
}

}





// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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