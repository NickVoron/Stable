// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "math/library.include.h"

namespace VariantFunctions
{
	template<class T0, class T1> struct TypePromoter
	{
		typedef typename TypePromoter<T1, T0>::Result Result;
	};

	template<class T> struct TypePromoter<T, T>			{ typedef T			Result;	};
	template<> struct TypePromoter<int, float>			{ typedef float		Result; };
	template<> struct TypePromoter<unsigned int, float> { typedef float		Result; };
	template<> struct TypePromoter<bool, float>			{ typedef float		Result; };
	template<> struct TypePromoter<bool, int>			{ typedef int		Result; };
	template<> struct TypePromoter<bool, unsigned int>  { typedef int		Result; };
	template<> struct TypePromoter<unsigned int, int>	{ typedef int		Result; };
	template<> struct TypePromoter<float, Vector2>		{ typedef Vector2	Result; };
	template<> struct TypePromoter<float, Vector3>		{ typedef Vector3	Result; };
	template<> struct TypePromoter<float, Vector4>		{ typedef Vector4	Result; };
	template<> struct TypePromoter<int, Vector2>		{ typedef Vector2	Result; };
	template<> struct TypePromoter<int, Vector3>		{ typedef Vector3	Result; };
	template<> struct TypePromoter<int, Vector4>		{ typedef Vector4	Result; };
	template<> struct TypePromoter<unsigned int, Vector2> { typedef Vector2	Result; };
	template<> struct TypePromoter<unsigned int, Vector3> { typedef Vector3	Result; };
	template<> struct TypePromoter<unsigned int, Vector4> { typedef Vector4	Result; };
}  




// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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