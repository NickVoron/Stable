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
