#pragma once

#include "swizzle.h"

namespace nm
{

	template<class scalar>
	swizzle2_t<scalar>::swizzle2_t(scalar& _x, scalar& _y)
	:x(_x), y(_y)
	{

	}

	template<class scalar>
	template<class vec_t>
	swizzle2_t<scalar>& swizzle2_t<scalar>::operator=(const vec_t& v)
	{
 		scalar _x = v.x;
 		scalar _y = v.y;
 		x = _x;
 		y = _y;
		return *this;
	}
	
	template<class scalar>
	template<class T>
	swizzle2_t<scalar>::operator T() const
	{
		return T(x, y);
	}

	template<class scalar>
	template<typename vec_t>
	void swizzle2_t<scalar>::operator +=(const vec_t& v)
	{
		scalar _x = v.x;
		scalar _y = v.y;
		x += _x;
		y += _y;		
	}

	template<class scalar>
	template<typename vec_t> 
	void swizzle2_t<scalar>::operator -=(const vec_t& v)
	{
		scalar _x = v.x;
		scalar _y = v.y;
		x -= _x;
		y -= _y;
	}
	
	template<class scalar>
	template<typename vec_t>
	void swizzle2_t<scalar>::operator *=(const vec_t& v)
	{
		scalar _x = v.x;
		scalar _y = v.y;
		x *= _x;
		y *= _y;
	}

	template<class scalar>
	template<typename vec_t>
	void swizzle2_t<scalar>::operator /=(const vec_t& v)
	{
		scalar _x = v.x;
		scalar _y = v.y;
		x /= _x;
		y /= _y;
	}
	

	template<class scalar>
	void swizzle2_t<scalar>::operator*=(scalar s)
	{
		x *= s;
		y *= s;
	}

	template<class scalar>
	void swizzle2_t<scalar>::operator/=(scalar s)
	{
		float is = 1.0f / s;
		x *= is;
		y *= is;
	}

}

