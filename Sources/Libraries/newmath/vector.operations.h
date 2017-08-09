// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "vector.impl.h"
#include "operations.h"

namespace nm
{
	template<class RES, class TC1, class TC2>
	inline void cross(const TC1& v1, const TC2& v2, RES& res)
	{	
		res.x = v1.y*v2.z - v1.z*v2.y;
		res.y = v1.z*v2.x - v1.x*v2.z;
		res.z = v1.x*v2.y - v1.y*v2.x;
	}

	template<class RES, class TC1, class TC2>
	inline RES cross(const TC1& v1, const TC2& v2)
	{	
		RES res;
		cross(v1, v2, res);
		return res;
	}

	
	template<class VT0_T, class VT1_T, bool v2, bool v3, bool v4> struct Add_impl;

	template<class VT0_T, class VT1_T> struct Add_impl<VT0_T, VT1_T, true, false, false> { typedef vector2_t<float> res; static inline res call(const VT0_T& v0, const VT1_T& v1) { return res(v0.x + v1.x, v0.y + v1.y); } };
	template<class VT0_T, class VT1_T> struct Add_impl<VT0_T, VT1_T, false, true, false> { typedef vector3_t<float> res; static inline res call(const VT0_T& v0, const VT1_T& v1) { return res(v0.x + v1.x, v0.y + v1.y, v0.z + v1.z); } };
	template<class VT0_T, class VT1_T> struct Add_impl<VT0_T, VT1_T, false, false, true> { typedef vector4_t<float> res; static inline res call(const VT0_T& v0, const VT1_T& v1) { return res(v0.x + v1.x, v0.y + v1.y, v0.z + v1.z, v0.w + v1.w); } };


	template<class VT0_T, class VT1_T>
	struct Add
	{
		static const bool vec2 = type_traits::is_vector2<VT0_T>::value && type_traits::is_vector2<VT1_T>::value;
		static const bool vec3 = type_traits::is_vector3<VT0_T>::value && type_traits::is_vector3<VT1_T>::value;
		static const bool vec4 = type_traits::is_vector4<VT0_T>::value && type_traits::is_vector4<VT1_T>::value;


		typedef typename Add_impl<VT0_T, VT1_T, vec2, vec3, vec4>::res result;
		static inline result call(const VT0_T& v0, const VT1_T& v1) { return Add_impl<VT0_T, VT1_T, vec2, vec3, vec4>::call(v0, v1); }
	};

	
	template<class VT0_T, class VT1_T, bool v2, bool v3, bool v4> struct Sub_impl;

	template<class VT0_T, class VT1_T> struct Sub_impl<VT0_T, VT1_T, true, false, false> { typedef vector2_t<float> res; static inline res call(const VT0_T& v0, const VT1_T& v1) { return vector2_t<float>(v0.x - v1.x, v0.y - v1.y); } };
	template<class VT0_T, class VT1_T> struct Sub_impl<VT0_T, VT1_T, false, true, false> { typedef vector3_t<float> res; static inline res call(const VT0_T& v0, const VT1_T& v1) { return vector3_t<float>(v0.x - v1.x, v0.y - v1.y, v0.z - v1.z); } };
	template<class VT0_T, class VT1_T> struct Sub_impl<VT0_T, VT1_T, false, false, true> { typedef vector4_t<float> res; static inline res call(const VT0_T& v0, const VT1_T& v1) { return vector4_t<float>(v0.x - v1.x, v0.y - v1.y, v0.z - v1.z, v0.w - v1.w); } };

	template<class VT0_T, class VT1_T>
	struct Sub
	{
		static const bool vec2 = type_traits::is_vector2<VT0_T>::value && type_traits::is_vector2<VT1_T>::value;
		static const bool vec3 = type_traits::is_vector3<VT0_T>::value && type_traits::is_vector3<VT1_T>::value;
		static const bool vec4 = type_traits::is_vector4<VT0_T>::value && type_traits::is_vector4<VT1_T>::value;


		typedef typename Sub_impl<VT0_T, VT1_T, vec2, vec3, vec4>::res result;
		static inline result call(const VT0_T& v0, const VT1_T& v1) { return Sub_impl<VT0_T, VT1_T, vec2, vec3, vec4>::call(v0, v1); }
	};

	
	template<class VT0_T, class VT1_T, bool v2, bool v3, bool v4> struct Dot_impl;

	template<class VT0_T, class VT1_T> struct Dot_impl<VT0_T, VT1_T, true, false, false> { typedef typename type_traits::is_vector2<VT0_T>::type res;  static inline res call(const VT0_T& v0, const VT1_T& v1) { return v0.x*v1.x + v0.y*v1.y; } };
	template<class VT0_T, class VT1_T> struct Dot_impl<VT0_T, VT1_T, false, true, false> { typedef typename type_traits::is_vector3<VT0_T>::type res;  static inline res call(const VT0_T& v0, const VT1_T& v1) { return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z; } };
	template<class VT0_T, class VT1_T> struct Dot_impl<VT0_T, VT1_T, false, false, true> { typedef typename type_traits::is_vector4<VT0_T>::type res;  static inline res call(const VT0_T& v0, const VT1_T& v1) { return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z + v0.w*v1.w; } };

	template<class VT0_T, class VT1_T>
	struct Dot
	{
		static const bool vec2 = type_traits::is_vector2<VT0_T>::value && type_traits::is_vector2<VT1_T>::value;
		static const bool vec3 = type_traits::is_vector3<VT0_T>::value && type_traits::is_vector3<VT1_T>::value;
		static const bool vec4 = type_traits::is_vector4<VT0_T>::value && type_traits::is_vector4<VT1_T>::value;


		typedef typename Dot_impl<VT0_T, VT1_T, vec2, vec3, vec4>::res result;
		static inline result call(const VT0_T& v0, const VT1_T& v1) { return Dot_impl<VT0_T, VT1_T, vec2, vec3, vec4>::call(v0, v1); }
	};

	
	template<class VT0_T, class S, bool v2, bool v3, bool v4> struct Scale_impl;

	template<class VT0_T, class S> struct Scale_impl<VT0_T, S, true, false, false> { typedef vector2_t<float> res; static inline res call(const VT0_T& v0, S s) { return res(v0.x * s, v0.y * s); } };
	template<class VT0_T, class S> struct Scale_impl<VT0_T, S, false, true, false> { typedef vector3_t<float> res; static inline res call(const VT0_T& v0, S s) { return res(v0.x * s, v0.y * s, v0.z * s); } };
	template<class VT0_T, class S> struct Scale_impl<VT0_T, S, false, false, true> { typedef vector4_t<float> res; static inline res call(const VT0_T& v0, S s) { return res(v0.x * s, v0.y * s, v0.z * s, v0.w * s); } };

	template<class VT0_T, class S>
	struct Scale
	{
		static const bool vec2 = type_traits::is_vector2<VT0_T>::value;
		static const bool vec3 = type_traits::is_vector3<VT0_T>::value;
		static const bool vec4 = type_traits::is_vector4<VT0_T>::value;


		typedef typename Scale_impl<VT0_T, S, vec2, vec3, vec4>::res result;
		static inline result call(const VT0_T& v0, S s) { return Scale_impl<VT0_T, S, vec2, vec3, vec4>::call(v0, s); }
	};


	template<class VT0_T, class VT1_T> inline typename Add<VT0_T, VT1_T>::result add(const VT0_T& a, const VT1_T& b) 		{ return Add<VT0_T, VT1_T>::call(a, b);	}
	template<class VT0_T, class VT1_T> inline typename Sub<VT0_T, VT1_T>::result sub(const VT0_T& a, const VT1_T& b) 		{ return Sub<VT0_T, VT1_T>::call(a, b);	}
	template<class VT0_T, class VT1_T> inline typename Dot<VT0_T, VT1_T>::result dot(const VT0_T& a, const VT1_T& b) 		{ return Dot<VT0_T, VT1_T>::call(a, b);	}
	template<class VT0_T> inline VT0_T scale(const VT0_T& a, float b)	{ return Scale<VT0_T, float>::call(a, b);	}

	template<class VectorType> inline typename Dot<VectorType, VectorType>::result magnitudeSq(const VectorType& v) { return dot(v, v); }
	template<class VectorType> inline float magnitude(const VectorType& v) { return sqrtf( magnitudeSq(v) );	}
	template<class VectorType> inline void normalize(VectorType& v) { v /= magnitude(v); }

	
	
	
	
	


	template<template <typename> class vec_t, typename scalar, typename value> inline vec_t<scalar> operator*(const vec_t<scalar>& a, value s) { return scale(a, (float)s);		}
	template<template <typename> class vec_t, typename scalar, typename value> inline vec_t<scalar> operator*(value s, const vec_t<scalar>& a) { return scale(a, (float) s);		}
	template<template <typename> class vec_t, typename scalar, typename value> inline vec_t<scalar> operator/(const vec_t<scalar>& a, value s) { return scale(a, 1.0f/ (float) s);	}

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