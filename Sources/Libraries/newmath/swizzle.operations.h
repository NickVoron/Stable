#pragma once

#include "vector.impl.h"
#include "swizzle.impl.h"

namespace nm
{

	//
	//
	//
	template<typename scalar>
	inline vector2_t<scalar> add(const swizzle2_t<scalar>& a, const swizzle2_t<scalar>& b)
	{
		return vector2_t<scalar>(a.x + b.x, a.y + b.y);
	}

	//
	//
	//
 	template<typename scalar>
 	inline vector2_t<scalar> sub(const swizzle2_t<scalar>& a, const swizzle2_t<scalar>& b)
 	{
 		return vector2_t<scalar>(a.x - b.x, a.y - b.y);
 	}

	//
	//
	//
	template<typename scalar>
	inline vector2_t<scalar> scale(const swizzle2_t<scalar>& a, const swizzle2_t<scalar>& b)
	{
		return vector2_t<scalar>(a.x * b.x, a.y * b.y);
	}

	template<typename scalar, typename value>
	inline vector2_t<scalar> scale(const swizzle2_t<scalar>& a, value s)
	{
		return vector2_t<scalar>(a.x * s, a.y * s);
	}

	//
	//
	//
	template<typename scalar>
	inline vector2_t<scalar> div(const swizzle2_t<scalar>& a, const swizzle2_t<scalar>& b)
	{
		return vector2_t<scalar>(a.x / b.x, a.y / b.y);
	}

	template<typename scalar, typename value>
	inline vector2_t<scalar> div(const swizzle2_t<scalar>& a, value s)
	{
		float is = 1.0f / s;
		return vector2_t<scalar>(a.x * is, a.y * is);
	}


	//
	//
	//
	template<typename scalar>
	inline vector2_t<scalar> operator+(const swizzle2_t<scalar>& a, const swizzle2_t<scalar>& b)
	{
		return add(a, b);
	}

	template<typename scalar>
	inline vector2_t<scalar> operator-(const swizzle2_t<scalar>& a, const swizzle2_t<scalar>& b)
	{
		return sub(a, b);
	}

	template<typename scalar>
	inline vector2_t<scalar> operator*(const swizzle2_t<scalar>& a, const swizzle2_t<scalar>& b)
	{
		return scale(a, b);
	}

	template<typename scalar, typename value>
	inline vector2_t<scalar> operator*(const swizzle2_t<scalar>& a, value s)
	{
		return scale(a, s);
	}

	template<typename scalar, typename value>
	inline vector2_t<scalar> operator*(value s, const swizzle2_t<scalar>& a)
	{
		return scale(a, s);
	}

	template<typename scalar>
	inline vector2_t<scalar> operator/(const swizzle2_t<scalar>& a, const swizzle2_t<scalar>& b)
	{
		return div(a, b);
	}

	template<typename scalar, typename value>
	inline vector2_t<scalar> operator/(const swizzle2_t<scalar>& a, value s)
	{
		return div(a, s);
	}



}





