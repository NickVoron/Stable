// Copyright (C) 2014-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <string>
#include "math/library.include.h"
#include "strings/library.include.h"

namespace Variant
{
	template<class T> struct TypeTraits
	{
		typedef T Type;
		static const char* name() { return typeid(T).name(); }
		static bool boolean(Type val) { return val != 0; }
	};

	template<> struct TypeTraits<std::string>				{ typedef std::string	Type;	static const char* name() { return "string"; }	static bool boolean(Type val) { return true; } };
	template<> struct TypeTraits<str::string32>				{ typedef str::string32	Type;	static const char* name() { return "string"; }	static bool boolean(Type val) { return true; } };
	template<> struct TypeTraits<Vector2>					{ typedef Vector2		Type;	static const char* name() { return "Vector2"; }	static bool boolean(Type val) { return false; } };
	template<> struct TypeTraits<Vector3>					{ typedef Vector3		Type;	static const char* name() { return "Vector3"; }	static bool boolean(Type val) { return false; } };
	template<> struct TypeTraits<Vector4>					{ typedef Vector4		Type;	static const char* name() { return "Vector4"; }	static bool boolean(Type val) { return false; } };

	template<class T>	struct IsAtomicType					{ static const bool value = std::is_arithmetic<typename std::decay<T>::type>::value || std::is_enum<typename std::decay<T>::type>::value; };
	template<>			struct IsAtomicType<std::string>	{ static const bool value = true; };
	template<>			struct IsAtomicType<str::string32>  { static const bool value = true; };


	template<class T>	struct IsConstantType				{ static const bool value = IsAtomicType<T>::value; };
	template<>			struct IsConstantType<Vector2>		{ static const bool value = true; };
	template<>			struct IsConstantType<Vector3>		{ static const bool value = true; };
	template<>			struct IsConstantType<Vector4>		{ static const bool value = true; };

}  



// Copyright (C) 2014-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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