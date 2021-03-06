// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "expression.h"
#include "holder.h"

#include "variantType/library.include.h"

#include <string>

namespace Expressions
{
	
	
	
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
		Expressions::EvaluationUnit* evaluated(Expressions::EvaluatedScope& environment) const override { return Expressions::add<Void>(*this);	}
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

 	inline ExpressionConst<std::string>* add_const_expression(const char* p0) { return add<ExpressionConst<std::string>>(p0); }
 	inline Expression* add_const_expression(Expression* p0) { return p0; }

	template<class P0>
	Const<P0>* add_const_unit(P0&& p0)
	{
		return (Const<P0>*)add_const_expression(p0)->evaluated(EvaluationUnit::commonParent);
	}

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