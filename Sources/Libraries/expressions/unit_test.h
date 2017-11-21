#pragma once

#include "expressions/library.include.h"
#include "unitTest/library.include.h"

template<class T0, class T1>
auto checkExpressionTypeToType(const T1 value)
{
	auto* expr = Expressions::convertType(value);
	T0 testvalue;
	Expressions::convertVar(*expr, testvalue);

//	LOG_MSG("test: " << Variant::TypeTraits<T1>::name() << " input: " << value << " " << Variant::TypeTraits<T0>::name() << " result: " << testvalue);
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