// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <unordered_set>

#include "strings/library.include.h"
#include "stuff/library.include.h"
#include "reflection/library.include.h"
#include "TGFParser/library.include.h"


#include "../expression.h"
#include "../holder.h"
#include "../const.h"
#include "../arrayContainer.h"
#include "../property.h"


namespace Expressions
{
	template<class T, bool is_arithmetic = std::is_arithmetic<typename std::decay<T>::type>::value || std::is_enum<T>::value>
	struct ConstExpression;

	template<class T>
	struct ConstExpression<T, true>
	{
		static Expression* convert(T value) { return Expressions::template add<Const<T>>(value); }
	};

	template<bool is_arithmetic>
	struct ConstExpression<std::string, is_arithmetic>
	{
		static Expression* convert(std::string value) { return Expressions::template add< Const<std::string> >(value); }
	};

	template<bool is_arithmetic>
	struct ConstExpression<str::string64, is_arithmetic>
	{
		static Expression* convert(str::string64 value) { return Expressions::template add<Const<std::string>>(value.c_str()); }
	};

	template<class T> struct IsConstant { static const bool value = (std::is_arithmetic<typename std::decay<T>::type>::value || std::is_enum<T>::value); };
	template<> struct IsConstant<std::string> { static const bool value = true; };
	template<> struct IsConstant<str::string64> { static const bool value = true; };


	template <class T> std::true_type has_value_type_d(typename T::value_type*);
	template <class T> std::false_type has_value_type_d(...);
	template <class T>	struct has_value_type : decltype(has_value_type_d<T>(nullptr)) {};

	template <typename C, bool B = has_value_type<C>::value>
	struct is_std_vector;

	template <class C>
	struct is_std_vector<C, false>
	{
		constexpr static bool value = false;
	};

	template <class C>
	struct is_std_vector<C, true>
	{
		constexpr static bool value = std::is_base_of<std::vector<typename C::value_type, typename C::allocator_type>, C>::value;
	};
	
	template<class ValueType, unsigned int selector = 
		(IsConstant<ValueType>::value ? CONSTANT : 0) | 
		(is_std_vector<ValueType>::value ? CONTAINER : 0) | 
		(!is_std_vector<ValueType>::value && !IsConstant<ValueType>::value ? STRUCTURE : 0)>
	struct ConvertType
	{
		static Expression* convert(const ValueType& value);
	};

	template<class T> Expression* convertType(const T& value);

	template<class Content>
	Expression* convertGraph(const tgf::Graph<Content>& graph)
	{
		auto nodes = add<ArrayContainer>(EvaluationUnit::commonParent);

		std::map<std::string, EvaluationUnit*> dict0;
		std::map<EvaluationUnit*, ArrayContainer*> dict1;
		for (auto& val : graph.nodes)
		{
			auto node = add<EvalPropertiesStruct>("Node");
			
			auto id = convertType(val->id)->evaluated(EvaluationUnit::commonParent);
			auto content = convertType(val->content)->evaluated(EvaluationUnit::commonParent);
			auto links = add<ArrayContainer>(EvaluationUnit::commonParent);

			dict0[val->id] = id;
			dict1[id] = links;

			node->add("id", id);
			node->add("content", content);
			node->add("links", links);

			nodes->add(node);
		}

		int idx = 0;
		for (auto& rib : graph.ribs)
		{
			if (rib->node0 && rib->node1)
			{
				
				++idx;
				auto links = dict1[dict0[rib->node0->id]];
				auto link = dict0[rib->node1->id];
				links->add(link);
			}			
		}

		auto properties = add<EvalPropertiesStruct>("Graph");

		properties->add("nodes", nodes);

		return properties;
	}

	template<class Content>	
	struct ConvertType<tgf::Graph<Content>, STRUCTURE>
	{ 
		static Expression* convert(const tgf::Graph<Content>& val) 
		{
			return convertGraph(val); 
		} 
	};

	template<class T>
	Expression* convertArray(const T& values)
	{
		ArrayContainer* result = add<ArrayContainer>(EvaluationUnit::commonParent);
		for (auto& val : values)
		{
			EvaluationUnit* unit = convertType(val)->cast<EvaluationUnit>();
			ENFORCE(unit);
			result->add(unit);
		}

		return result;
	}

	template<>	struct ConvertType<Expression*> { static Expression* convert(Expression* value) { return (Expression*) value; } };

	template<class T>
	Expression* convertType(const T& value);

	template<class ValueType>
	struct ConvertType<ValueType, CONSTANT>
	{
		static Expression* convert(const ValueType& value) 
		{ 
			return ConstExpression<ValueType>::convert(value); 
		}
	};

	template<class ValueType>
	struct ConvertType<ValueType, STRUCTURE>
	{
		static Expression* convert(const ValueType& val)
		{
			auto value = mirror::type(val);
			auto& name = value.name();
			return Expressions::add<Struct>(name, ConstExprList(add_const(val)));
		}
	};

	template<class ValueType>	
	struct ConvertType<ValueType, CONTAINER>
	{ 
		static Expression* convert(const ValueType& val) 
		{ 
			return convertArray(val); 
		} 
	};

	template<class T>
	Expression* convertType(const T& value)
	{
		return ConvertType<T>::convert(value);
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