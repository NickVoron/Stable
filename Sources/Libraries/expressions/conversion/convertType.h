#pragma once

#include <type_traits>

#include "strings/library.include.h"
#include "reflection/library.include.h"
#include "TGFParser/library.include.h"

#include "common/type_traits.h"


#include "../expression.h"
#include "../holder.h"
#include "../const.h"
#include "../property.h"
#include "../typeTraits.h"


namespace Expressions
{
	template<class T>
	EvaluationUnit* convertType(const T& value);

	template<class T, bool is_arithmetic = std::is_arithmetic<typename std::decay<T>::type>::value || std::is_enum<T>::value>
	struct ConstExpression;

	template<class T>
	struct ConstExpression<T, true>
	{
		static EvaluationUnit* convert(T value) { return add_const_unit(value); }
	};

	template<bool is_arithmetic>
	struct ConstExpression<const char*, is_arithmetic>
	{
		static EvaluationUnit* convert(const char* value) { return add_const_unit(value); }
	};

	template<bool is_arithmetic>
	struct ConstExpression<const wchar_t*, is_arithmetic>
	{
		static EvaluationUnit* convert(const wchar_t* value) { return add_const_unit(value); }
	};

	template<bool is_arithmetic>
	struct ConstExpression<std::string, is_arithmetic>
	{
		static EvaluationUnit* convert(std::string value) { return add_const_unit(value); }
	};

	template<bool is_arithmetic>
	struct ConstExpression<str::string64, is_arithmetic>
	{
		static EvaluationUnit* convert(str::string64 value) { return add_const_unit(value.c_str()); }
	};

	template<class T>
	struct PairExpression
	{
		static EvaluationUnit* convert(T value) 
		{ 
			auto protostruct = Expressions::template add<Struct>("pair", ConstExprList(value.first, value.second));
			return protostruct->evaluated(EvaluationUnit::commonParent);
		}
	};

	template<class T>
	struct TupleExpression
	{
		template<class... P>
		static EvaluationUnit* convert(std::tuple<P...> value) 
		{ 
			ConstExprList params;
			stl::for_each(value, [&params](auto& element) { params.add(element); });
			return Expressions::template add<Struct>("tuple", ConstExprList(params));
		}
	};
	
	template<class ValueType, unsigned int selector = 
		(stl::IsConstant<ValueType>::value    ? CONSTANT  : 0) | 
		(stl::IsContainer<ValueType>::value   ? CONTAINER : 0) |
		(stl::IsStructure<ValueType>::value   ? STRUCTURE : 0) |
		(stl::is_std_pair<ValueType>::value   ? PAIR      : 0) |
		(stl::is_std_tuple<ValueType>::value  ? TUPLE     : 0) >
	struct ConvertType
	{
		static EvaluationUnit* convert(const ValueType& value);
	};

	template<class Content>
	EvaluationUnit* convertGraph(const tgf::Graph<Content>& graph)
	{
		auto proto = add<Array>();
		auto nodes = proto->evaluated(EvaluationUnit::commonParent)->cast<ArrayContainer>();

		std::map<std::string, EvaluationUnit*> dict0;
		std::map<EvaluationUnit*, ArrayContainer*> dict1;
		auto nodeproto = add<PropertiesStruct>("Node");
		for (auto& val : graph.nodes)
		{
			auto node = nodeproto->evaluated(EvaluationUnit::commonParent)->cast<PropertiesStructUnit>();
			
			auto id = convertType(val->id);
			auto content = convertType(val->content);
			auto links = proto->evaluated(EvaluationUnit::commonParent)->cast<ArrayContainer>();

			dict0[val->id] = id;
			dict1[id] = links;

			node->add("id", id);
			node->add("content", content);
			node->add("links", links);

			nodes->add(node);
		}

		int idx = 0;
		for (auto& edge : graph.edges)
		{
			if (edge->node0 && edge->node1)
			{
				//LOG_EXPRESSION(idx, rib->node0, rib->node1);
				++idx;
				auto links = dict1[dict0[edge->node0->id]];
				auto link = dict0[edge->node1->id];
				links->add(link);
			}			
		}

		auto graphproto = add<PropertiesStruct>("Graph");
		auto properties = graphproto->evaluated(EvaluationUnit::commonParent)->cast<PropertiesStructUnit>();

		properties->add("nodes", nodes);

		return properties;
	}

	template<class Content>	
	struct ConvertType<tgf::Graph<Content>, STRUCTURE>
	{ 
		static EvaluationUnit* convert(const tgf::Graph<Content>& val) 
		{
			return convertGraph(val); 
		} 
	};


	template<class T>
	EvaluationUnit* convertContainer(const T& values)
	{
		auto proto = add<Array>();
		ArrayContainer* result = proto->evaluated(EvaluationUnit::commonParent)->cast<ArrayContainer>();
		for (auto& val : values)
		{
			EvaluationUnit* unit = convertType(val);
			ENFORCE(unit);
			result->add(unit);
		}

		return result;
	}

	template<>	
	struct ConvertType<EvaluationUnit*> 
	{ 
		static EvaluationUnit* convert(EvaluationUnit* value) { return value; } 
	};

	template<class ValueType>
	struct ConvertType<ValueType, CONSTANT>
	{
		static EvaluationUnit* convert(const ValueType& value) 
		{ 
			return ConstExpression<ValueType>::convert(value); 
		}
	};

	template<class ValueType>
	struct ConvertType<ValueType, STRUCTURE>
	{
		static EvaluationUnit* convert(const ValueType& val)
		{
			auto value = mirror::type(val);
			auto& name = value.name();
			auto protostruct = Expressions::add<Struct>(name, ConstExprList(add_const_expression(val)));
			return protostruct->evaluated(EvaluationUnit::commonParent);
		}
	};

	template<class ValueType>	
	struct ConvertType<ValueType, CONTAINER>
	{ 
		static EvaluationUnit* convert(const ValueType& val) 
		{ 
			return convertContainer(val); 
		} 
	};

	template<class ValueType>
	struct ConvertType<ValueType, PAIR>
	{
		static EvaluationUnit* convert(const ValueType& val)
		{
			return PairExpression<ValueType>::convert(val);
		}
	};

	template<class ValueType>
	struct ConvertType<ValueType, TUPLE>
	{
		static EvaluationUnit* convert(const ValueType& val)
		{
			return TupleExpression<ValueType>::convert(val);
		}
	};

	template<class T>
	EvaluationUnit* convertType(const T& value)
	{
		return ConvertType<T>::convert(value);
	}	
}




