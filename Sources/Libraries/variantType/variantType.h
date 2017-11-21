#pragma once

#include "typeTraits.h"

namespace Variant
{
	struct Value 
	{
		template <class TypeExtractor, class T>
		bool result(T& value) const
		{ 
			return TypeExtractor::template extract<T>(this, value);
		}

		virtual ~Value() {}
		virtual bool boolean() const { return true; }
		virtual std::string typeName() const { return "unknown operand type"; }
	};

	//
	struct ValueList : public std::vector<Value*>
	{
		template<class... Val> explicit ValueList(Val*... v) : vector{ v... } { }
	};


	template<class ValueType>
	class Const : public Value
	{
	public:
		Const(const ValueType& value_) : value(value_) {}

		const ValueType value;

		virtual bool boolean() const { return TypeTraits<ValueType>::boolean(value); }
		virtual std::string typeName() const { return TypeTraits<ValueType>::name(); }
	};

	template<>
	class Const<void> : public Value
	{
	public:
		virtual std::string typeName() const { return "void"; }
	};

	template<class ValueType>
	class Pointer : public Value
	{
	public:
		Pointer(ValueType* value_) : value(value_) {}

		ValueType* value;

		virtual bool boolean() const { return value != nullptr; }
		virtual std::string typeName() const { return "pointer"; }
	};

}