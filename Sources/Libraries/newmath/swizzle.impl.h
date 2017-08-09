// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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