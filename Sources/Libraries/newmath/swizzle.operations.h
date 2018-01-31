// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "vector.impl.h"
#include "swizzle.impl.h"

namespace nm
{

	
	
	
	template<typename scalar>
	inline vector2_t<scalar> add(const swizzle2_t<scalar>& a, const swizzle2_t<scalar>& b)
	{
		return vector2_t<scalar>(a.x + b.x, a.y + b.y);
	}

	
	
	
 	template<typename scalar>
 	inline vector2_t<scalar> sub(const swizzle2_t<scalar>& a, const swizzle2_t<scalar>& b)
 	{
 		return vector2_t<scalar>(a.x - b.x, a.y - b.y);
 	}

	
	
	
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









// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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