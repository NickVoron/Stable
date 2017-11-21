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
		static bool boolean(Type val) { return false; }
	};

	template<> struct TypeTraits<std::string>				{ typedef std::string	Type;	static const char* name() { return "string"; }	static bool boolean(Type val) { return true; } };
	
	template<> struct TypeTraits<Vector2>					{ typedef Vector2		Type;	static const char* name() { return "Vector2"; }	static bool boolean(Type val) { return false; } };
	template<> struct TypeTraits<Vector3>					{ typedef Vector3		Type;	static const char* name() { return "Vector3"; }	static bool boolean(Type val) { return false; } };
	template<> struct TypeTraits<Vector4>					{ typedef Vector4		Type;	static const char* name() { return "Vector4"; }	static bool boolean(Type val) { return false; } };

	template<class CT, std::size_t size> struct TypeTraits<str::string_t<CT, size>> { typedef str::string_t<CT, size>	Type;	static const char* name() { return "string"; }	static bool boolean(Type val) { return true; } };

	template<class T>	struct IsAtomicType					{ static const bool value = std::is_arithmetic<typename std::decay<T>::type>::value || std::is_enum<typename std::decay<T>::type>::value; };
	template<>			struct IsAtomicType<std::string>	{ static const bool value = true; };
	template<class CT, std::size_t size> struct IsAtomicType<str::string_t<CT, size>>  { static const bool value = true; };


	template<class T>	struct IsConstantType				{ static const bool value = IsAtomicType<T>::value; };
	template<>			struct IsConstantType<Vector2>		{ static const bool value = true; };
	template<>			struct IsConstantType<Vector3>		{ static const bool value = true; };
	template<>			struct IsConstantType<Vector4>		{ static const bool value = true; };

}  