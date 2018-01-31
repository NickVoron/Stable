// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "expressions/library.include.h"
#include "unitTest/library.include.h"

template<class T0, class T1>
auto checkExpressionTypeToType(const T1 value)
{
	auto* expr = Expressions::convertType(value);
	T0 testvalue;
	Expressions::convertVar(*expr, testvalue);


	TEST_CHECK(testvalue == value);
	return expr;
}

template<typename... T, size_t... I>
decltype(auto) sequence_test_impl(std::index_sequence<I...>)
{
	return std::make_tuple(checkExpressionTypeToType<T>((T) I)...);
}

template<typename... P>
decltype(auto) sequence_test()
{
	return sequence_test_impl<std::decay_t<P>...>(std::make_index_sequence<sizeof...(P)>{});
}

template<typename... T, size_t... I, typename V>
decltype(auto) sequence_test_impl_promotion(std::index_sequence<I...>, V value)
{
	return std::make_tuple(checkExpressionTypeToType<T>(value)...);
}

template<typename... P, typename V>
decltype(auto) sequence_test_promotion(V value)
{
	return sequence_test_impl_promotion<std::decay_t<P>...>(std::make_index_sequence<sizeof...(P)>{}, value);
}

template<typename... P>
decltype(auto) sequence_promotion()
{
	return  std::make_tuple(sequence_test_promotion<P...>(P(123))...);
}

TEST_SUITE(ExpressionsTest)

TEST_CASE(ConstantsConversion)
{
	sequence_test<
		char,
		short,
		int,
		long,
		long long,
		unsigned char,
		unsigned short,
		unsigned int,
		unsigned long,
		unsigned long long,
		double,
		float
	>();

	sequence_promotion<
		char,
		short,
		int,
		long,
		long long,
		unsigned char,
		unsigned short,
		unsigned int,
		unsigned long,
		unsigned long long,
		double,
		float
	>();
}

enum TestEnum
{
	ENUM_VALUE_0,
	ENUM_VALUE_1,
	ENUM_VALUE_2,
};

enum class TestEnumClass
{
	ENUM_CLASS_VALUE_0,
	ENUM_CLASS_VALUE_1,
	ENUM_CLASS_VALUE_2,
};

std::ostream& operator<<(std::ostream& os, TestEnumClass value)
{
	return os << (int) value;
}

TEST_CASE(EnumConversion)
{
	{
		TestEnum value = ENUM_VALUE_1;
		checkExpressionTypeToType<TestEnum>(value);
	}

	{
		TestEnumClass value0 = TestEnumClass::ENUM_CLASS_VALUE_1;
		checkExpressionTypeToType<TestEnumClass>(value0);
	}
}

TEST_SUITE_END()



// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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