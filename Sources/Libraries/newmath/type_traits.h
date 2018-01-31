// Copyright (C) 2012-2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "libcfg.h"

#pragma pack(push,1)
namespace nm{
namespace type_traits{

	namespace internal
	{
		struct YES	{ char d[1]; };
		struct NO	{ char d[2]; };

		struct unknown_t{};

		template<class T, T value> struct wrapper {};
		template <class T> T& make();

		template <bool b, class T1, class T2> struct conditional			{ typedef T1 type; };
		template <class T1, class T2>struct conditional<false, T1, T2> 		{ typedef T2 type; };

		struct negative	{ static bool const value = false;	};




















#define HAS_MEMBER_T(N, M) \
		template <class T, class S> struct N{ \
		template <class U> struct test{	static YES &conversion(S T::*);	static NO &conversion(...);	static bool const value = sizeof conversion(&U::M) == sizeof(YES);};\
		struct explicit_member { S M; };\
		struct help : public explicit_member, public T{};\
		template <typename U>	static NO has(U&, wrapper<S explicit_member::*, &U::M>* = 0);\
		static YES has(...);\
		static const bool value = conditional<sizeof(YES) == sizeof(has(make<help>())),	test<T>, negative>::type::value;\
		};

  		HAS_MEMBER_T(has_element_x, x);
  		HAS_MEMBER_T(has_element_y, y);
  		HAS_MEMBER_T(has_element_z, z);
  		HAS_MEMBER_T(has_element_w, w);
  #undef HAS_MEMBER_T

		template <class T, class S>
		struct v2_t
		{
			static const bool value =	has_element_x<T, S>::value &&
										has_element_y<T, S>::value &&
										sizeof(T) == sizeof(S) * 2;
		};

 		template <class T, class S>
 		struct v3_t
 		{
 			static const bool value =	has_element_x<T, S>::value &&
  										has_element_y<T, S>::value &&
  										has_element_z<T, S>::value &&
  										sizeof(T) == sizeof(S) * 3;
 		};

 		template <class T, class S>
 		struct v4_t
 		{
 			static const bool value =	has_element_x<T, S>::value &&
 										has_element_y<T, S>::value &&
 										has_element_z<T, S>::value &&
 										has_element_w<T, S>::value &&
 										sizeof(T) == sizeof(S) * 4;
 		};

#define DEFINE_CHECKER(CHECKER, VT)\
		template <class T>\
		struct CHECKER\
		{\
			static const bool value =	VT<T, float>::value || VT<T, double>::value ||			\
										VT<T, int>::value	|| VT<T, unsigned int>::value ||	\
										VT<T, char>::value	|| VT<T, unsigned char>::value ||	\
										VT<T, short>::value || VT<T, unsigned short>::value;	\
		};

		DEFINE_CHECKER(v2_check, v2_t);
		DEFINE_CHECKER(v3_check, v3_t);
		DEFINE_CHECKER(v4_check, v4_t);


		template <bool is_float, bool is_double, bool is_int, bool is_uint, bool is_char, bool is_uchar, bool is_short, bool is_ushort>
		struct determinator_impl
		{
			typedef unknown_t result;
		};

		template <>	struct determinator_impl<true, false, false, false, false, false, false, false> { typedef float				result; };
		template <>	struct determinator_impl<false, true, false, false, false, false, false, false> { typedef double			result; };
		template <>	struct determinator_impl<false, false, true, false, false, false, false, false> { typedef int				result; };
		template <>	struct determinator_impl<false, false, false, true, false, false, false, false> { typedef unsigned int		result; };
		template <>	struct determinator_impl<false, false, false, false, true, false, false, false> { typedef char				result; };
		template <>	struct determinator_impl<false, false, false, false, false, true, false, false> { typedef unsigned char		result; };
		template <>	struct determinator_impl<false, false, false, false, false, false, true, false> { typedef short				result; };
		template <>	struct determinator_impl<false, false, false, false, false, false, false, true> { typedef unsigned short	result; };

		template<class T, bool v2 = v2_check<T>::value, bool v3 = v3_check<T>::value, bool v4 = v4_check<T>::value>
		struct determinator_vec_range
		{
			static const int value = 0;
		};

		template<class T>	struct determinator_vec_range<T, true, false, false> { static const int value = 2; };
		template<class T>	struct determinator_vec_range<T, false, true, false> { static const int value = 3; };
		template<class T>	struct determinator_vec_range<T, false, false, true> { static const int value = 4; };



		template <class T>
		struct determinator_t
		{
			static const int elementsCount = determinator_vec_range<T>::value;

			typedef determinator_impl<	has_element_x<T, float>::value,
							has_element_x<T, double>::value,
							has_element_x<T, int>::value,
							has_element_x<T, unsigned int>::value,
							has_element_x<T, char>::value,
							has_element_x<T, unsigned char>::value,
							has_element_x<T, short>::value,
							has_element_x<T, unsigned short>::value> res_impl;

			typedef typename res_impl::result result;


		};
	}

	template <class T>	struct is_vector2  { static const bool value = internal::v2_check<T>::value; typedef typename internal::determinator_t<T>::result type; };
	template <class T>	struct is_vector3  { static const bool value = internal::v3_check<T>::value; typedef typename internal::determinator_t<T>::result type; };
	template <class T>	struct is_vector4  { static const bool value = internal::v4_check<T>::value; typedef typename internal::determinator_t<T>::result type; };

}

}

#pragma pack(pop)




// Copyright (C) 2012-2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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