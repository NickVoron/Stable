// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "../functions.h"
#include "../typePromotion.h"

#include "math/library.include.h"

namespace VariantFunctions
{
	
	
	
	
	template<class T> struct Rand2;
	template<> struct Rand2<float> { static float call(float v1, float v2) { return Base::MathUtils::GetRandomFromRange(v1, v2); } };
	template<> struct Rand2<Vector2> { static Vector2 call(Vector2 v1, Vector2 v2) { return Vector2(Rand2<float>::call(v1.x, v2.x), Rand2<float>::call(v1.y, v2.y)); } };
	template<> struct Rand2<Vector3> { static Vector3 call(Vector3 v1, Vector3 v2) { return Vector3(Rand2<float>::call(v1.x, v2.x), Rand2<float>::call(v1.y, v2.y), Rand2<float>::call(v1.z, v2.z)); } };
	template<> struct Rand2<Vector4> { static Vector4 call(Vector4 v1, Vector4 v2) { return Vector4(Rand2<float>::call(v1.x, v2.x), Rand2<float>::call(v1.y, v2.y), Rand2<float>::call(v1.z, v2.z), Rand2<float>::call(v1.w, v2.w)); } };
	template<> struct Rand2<int> { static int call(int v1, int v2) { return Base::MathUtils::GetRandomFromRange(v1, v2); } };
	template<> struct Rand2<bool> { static bool call(bool v1, bool v2) { return Base::MathUtils::GetRandomTrigger(0.5f); } };

	template<class T> struct Rand0;
	template<> struct Rand0<float> { static float call() { return Base::MathUtils::GetRandomThetha(); } };
	template<> struct Rand0<Vector2> { static Vector2 call() { return Vector2(Rand0<float>::call(), Rand0<float>::call()); } };
	template<> struct Rand0<Vector3> { static Vector3 call() { return Vector3(Rand0<float>::call(), Rand0<float>::call(), Rand0<float>::call()); } };
	template<> struct Rand0<Vector4> { static Vector4 call() { return Vector4(Rand0<float>::call(), Rand0<float>::call(), Rand0<float>::call(), Rand0<float>::call()); } };
	template<> struct Rand0<int> { static int call() { return rand(); } };
	template<> struct Rand0<bool> { static bool call() { return Base::MathUtils::GetRandomTrigger(0.5f); } };


	template<class VariantType, class VariantTypeList, template <class> class TypeToName, class VariantCreator, class VariantName, class VariantConverter, template <class> class ValueFunctionCaller>
	struct CommonFunctions 
	{
		typedef Types<VariantType, VariantTypeList, TypeToName, VariantCreator, VariantName, VariantConverter, ValueFunctionCaller> FunctionTypes;

		template<class T0, class T1> struct less		{ typedef typename TypePromoter<T0, T1>::Result T; static inline bool call(T0 v0, T1 v1) { return (T) v0 <  (T) v1; } };
		template<class T0, class T1> struct greater		{ typedef typename TypePromoter<T0, T1>::Result T; static inline bool call(T0 v0, T1 v1) { return (T) v0 >  (T) v1; } };
		template<class T0, class T1> struct lequal		{ typedef typename TypePromoter<T0, T1>::Result T; static inline bool call(T0 v0, T1 v1) { return (T) v0 <= (T) v1; } };
		template<class T0, class T1> struct gequal		{ typedef typename TypePromoter<T0, T1>::Result T; static inline bool call(T0 v0, T1 v1) { return (T) v0 >= (T) v1; } };
		template<class T0, class T1> struct equal		{ typedef typename TypePromoter<T0, T1>::Result T; static inline bool call(T0 v0, T1 v1) { return (T) v0 == (T) v1; } };
		template<class T0, class T1> struct notequal	{ typedef typename TypePromoter<T0, T1>::Result T; static inline bool call(T0 v0, T1 v1) { return (T) v0 != (T) v1; } };

		template<class T0, class T1> struct AddImpl	{ static inline typename TypePromoter<T0, T1>::Result call(T0 v0, T1 v1) { return v0 + v1; } };
		template<class T0, class T1> struct SubImpl	{ static inline typename TypePromoter<T0, T1>::Result call(T0 v0, T1 v1) { return v0 - v1; } };
		template<class T0, class T1> struct MulImpl	{ static inline typename TypePromoter<T0, T1>::Result call(T0 v0, T1 v1) { return v0 * v1; } };
		template<class T0, class T1> struct DivImpl	{ static inline typename TypePromoter<T0, T1>::Result call(T0 v0, T1 v1) { return v0 / v1; } };
		template<class T0, class T1> struct ModImpl	{ static inline typename TypePromoter<T0, T1>::Result call(T0 v0, T1 v1) { return v0 % v1; } };
		template<class T> struct MagnitudeImpl	{ static inline float call(T v0) { return nm::magnitude(v0); } };

		template<class T0, class T1> struct AndImpl	{ static inline bool call(T0 v0, T1 v1) { return v0 && v1; } };
		template<class T0, class T1> struct OrImpl	{ static inline bool call(T0 v0, T1 v1) { return v0 || v1; } };
		template<class T> struct NotImpl { static inline bool call(T v0) { return !v0; } };

		template< template<class, class> class Impl, class FunctionType >
		struct Comparsion : public FunctionTypes::template Binary<FunctionType>
		{
			static void init()
			{
				Comparsion::template addSimple<int, int, Impl>();
				Comparsion::template addSimple<float, float, Impl>();
				Comparsion::template addSimple<std::string, std::string, Impl>();
				Comparsion::template addCommutative<int, float, Impl>();
				Comparsion::template addCommutative<int, unsigned int, Impl>();
				Comparsion::template addCommutative<int, bool, Impl>();
				Comparsion::template addCommutative<float, bool, Impl>();
			}
		};

		struct Less : public Comparsion<less, Less>		{};
		struct Greater : public Comparsion<greater, Greater>	{};
		struct Lequal : public Comparsion<lequal, Lequal>	{};
		struct Gequal : public Comparsion<gequal, Gequal>	{};
		struct Equal : public Comparsion<equal, Equal>	{};
		struct Notequal : public Comparsion<notequal, Notequal>	{};


		template<template<class, class> class Impl, class FunctionType>
		struct LogicBinary : public FunctionTypes::template Binary<FunctionType>
		{
			static void init()
			{
				LogicBinary::template addSimple<int, int, Impl>();
				LogicBinary::template addSimple<bool, bool, Impl>();
				LogicBinary::template addCommutative<int, bool, Impl>();
			}
		};

		struct And : public LogicBinary<AndImpl, And> {};
		struct Or : public LogicBinary<OrImpl, Or> {};
		struct Not : public FunctionTypes::template Unary<Not>
		{
			static void init()
			{
				Not::template add<int, NotImpl>();
				Not::template add<bool, NotImpl>();
			}
		};

		struct Add : public FunctionTypes::template Binary<Add>
		{
			static void init()
			{
				Add::template addSimple<std::string, std::string, AddImpl>();
				Add::template addSimple<int, int, AddImpl>();
				Add::template addCommutative<int, float, AddImpl>();
				Add::template addCommutative<int, unsigned int, AddImpl>();
				Add::template addSimple<Vector2, Vector2, AddImpl>();
				Add::template addSimple<Vector3, Vector3, AddImpl>();
				Add::template addSimple<Vector4, Vector4, AddImpl>();
			}
		};

		struct Sub : public FunctionTypes::template Binary<Sub>
		{
			static void init()
			{
				Sub::template addSimple<int, int, SubImpl>();
				Sub::template addCommutative<int, unsigned int, SubImpl>();
				Sub::template addCommutative<int, float, SubImpl>();
				Sub::template addSimple<Vector2, Vector2, SubImpl>();
				Sub::template addSimple<Vector3, Vector3, SubImpl>();
				Sub::template addSimple<Vector4, Vector4, SubImpl>();
			}
		};

		struct Mul : public FunctionTypes::template Binary<Mul>
		{
			static void init()
			{
				Mul::template addSimple<int, int, MulImpl>();
				Mul::template addCommutative<int, unsigned int, MulImpl>();
				Mul::template addCommutative<int, float, MulImpl>();
				Mul::template addCommutative<int, Vector2, MulImpl>();
				Mul::template addCommutative<int, Vector3, MulImpl>();
				Mul::template addCommutative<int, Vector4, MulImpl>();
				Mul::template addCommutative<float, Vector2, MulImpl>();
				Mul::template addCommutative<float, Vector3, MulImpl>();
				Mul::template addCommutative<float, Vector4, MulImpl>();
			}

		};
		struct Div : public FunctionTypes::template Binary<Div>
		{
			static void init()
			{
				Div::template addSimple<int, int, DivImpl>();
				Div::template addCommutative<int, unsigned int, DivImpl>();
				Div::template addCommutative<int, float, DivImpl>();
				Div::template addSimple<Vector2, int, DivImpl>();
				Div::template addSimple<Vector3, int, DivImpl>();
				Div::template addSimple<Vector4, int, DivImpl>();
				Div::template addSimple<Vector2, float, DivImpl>();
				Div::template addSimple<Vector3, float, DivImpl>();
				Div::template addSimple<Vector4, float, DivImpl>();
			}	

		};

		struct Mod : public FunctionTypes::template Binary<Mod>
		{
			static void init()
			{
				Mod::template addSimple<int, int, ModImpl>();
			}
		};

		struct Magnitude : public FunctionTypes::template Unary<Magnitude>
		{
			static void init()
			{
				Magnitude::template add<Vector2, MagnitudeImpl>();
				Magnitude::template add<Vector3, MagnitudeImpl>();
				Magnitude::template add<Vector4, MagnitudeImpl>();
			}
		};

		struct RandBinary : public FunctionTypes::template Binary<RandBinary>
		{
			static void init()
			{
				RandBinary::template addSimple1<int, Rand2>();
				RandBinary::template addSimple1<float, Rand2>();
				RandBinary::template addSimple1<Vector2, Rand2>();
				RandBinary::template addSimple1<Vector3, Rand2>();
				RandBinary::template addSimple1<Vector4, Rand2>();
			}
		};

		struct RandSimple : public FunctionTypes::template Simple<RandSimple>
		{
			static void init()
			{
				RandSimple::template add< Rand0<int> >();
				RandSimple::template add< Rand0<float> >();
				RandSimple::template add< Rand0<Vector2> >();
				RandSimple::template add< Rand0<Vector3> >();
				RandSimple::template add< Rand0<Vector4> >();
			}
		};


		struct Functions : public FunctionTypes::Functions
		{
			static void init()
			{
				Functions::add("less", &Less::call);
				Functions::add("greater", &Greater::call);
				Functions::add("lequal", &Lequal::call);
				Functions::add("gequal", &Gequal::call);
				Functions::add("equal", &Equal::call);
				Functions::add("notequal", &Notequal::call);

				Functions::add("and", &And::call);
				Functions::add("or", &Or::call);
				Functions::add("not", &Not::call);

				Functions::add("add", &Add::call);
				Functions::add("mul", &Mul::call);
				Functions::add("sub", &Sub::call);
				Functions::add("div", &Div::call);
				Functions::add("mod", &Mod::call);
				Functions::add("magnitude", &Magnitude::call);

				Functions::add("rand", &RandBinary::call);
			}
		};
	};
}



// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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