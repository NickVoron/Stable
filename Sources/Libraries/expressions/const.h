#pragma once
#include "expression.h"
#include "holder.h"

#include "variantType/library.include.h"

#include <string>

namespace Expressions
{
	//
	//
	//
	class Void : public EvaluationUnit
	{
	public:
		Void(const Expression& proto) : EvaluationUnit("void", proto, commonParent) {}

		virtual std::string string() const override { return "void"; }
		virtual std::string typeName() const override { return "void_t"; }
	};

	class VoidExpression : public Expression
	{
	public:
		Expressions::EvaluationUnit* VoidExpression::evaluated(Expressions::EvaluatedScope& environment) const override { return Expressions::add<Void>(*this);	}
		virtual std::string string() const override	{ return "void"; }
	};

	template<class ValueType>
	class Const : public EvaluationUnit
	{
	public:
		static_assert(!std::is_pointer_v<ValueType>);
		typedef ValueType Type;

		Const(const Expression& proto, const Type &value_) :	EvaluationUnit("const", proto, commonParent), value(value_) {}
		virtual bool boolean() const{ return Variant::TypeTraits<Type>::boolean(value); }
		virtual std::string string() const override	{ return str::stringize(value).str(); }
		virtual std::string typeName() const override { return Variant::TypeTraits<Type>::name(); }

		virtual std::unique_ptr<mirror::runtime::Type> reflectedType() const override { return mirror::type(value).clone(); }

		const Type value;
	};

	template<class ValueType>
	class ExpressionConst : public Expression
	{
	public:
		static_assert(!std::is_pointer_v<ValueType>);

		typedef ValueType Type;

		ExpressionConst(const Type &value_) : value(value_) {}

		virtual std::string string() const override { return str::stringize(value).str(); }
		virtual std::string typeName() const override { return Variant::TypeTraits<Type>::name(); }
		virtual EvaluationUnit* evaluated(EvaluatedScope& environment) const override
		{
			return add<Const<ValueType>>(*this, value);
		}

		const Type value;
	};

	template<class P0
		, typename = std::enable_if_t<!std::is_base_of_v<EvaluationUnit, std::remove_pointer_t<P0>>>
		, typename = std::enable_if_t<!std::is_base_of_v<Expression, std::remove_pointer_t<P0>>>
	>
	ExpressionConst<P0>* add_const_expression(const P0& p0) { return add<ExpressionConst<P0>>(p0); }
	
 	template<class P0>
 	ExpressionConst<P0>* add_const_expression(const Const<P0>* p0) 
 	{ 
 		return add_const_expression(p0->value);
 	}
// 
 	inline ExpressionConst<std::string>* add_const_expression(const char* p0) { return add<ExpressionConst<std::string>>(p0); }
 	inline Expression* add_const_expression(Expression* p0) { return p0; }

	template<class P0>
	Const<P0>* add_const_unit(const P0& p0)
	{
		return (Const<P0>*)add_const_expression(p0)->evaluated(EvaluationUnit::commonParent);
	}

}//
