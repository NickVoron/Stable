#pragma once

#include "../classDescEtc/propertyAssignment.h"

#include "expressions/library.include.h"

namespace Expressions
{
	using namespace ObjectParser;
	//
	//
	//
	template<class Ptr>
	inline const Expression* unptr(const Ptr& ptr)
	{
		return ptr;
	}

	template<>
	inline const Expression* unptr(const std::unique_ptr<PropertyAssignment>& ptr)
	{
		return ptr.get()->value;
	}

	template<>
	inline const Expression* extrexpr(const std::unique_ptr<const PropertyAssignment>& value)
	{
		return unptr(value->value);
	}

	template<>
	inline void preparePlaceholder<PropertyAssignmentList, EvaluatedScope>(const PropertyAssignmentList& source, EvaluatedScope& target)
	{
		for (auto& prop : source)
		{
			if (target.find(prop->propertyName) == target.end())
			{
				target.add(prop->propertyName, nullptr, InsertMethod::INSERT);
			}			
		}
	}

	template<>
	inline void preparePlaceholder<ExpressionScope, EvaluatedScope>(const ExpressionScope& source, EvaluatedScope& target)
	{
		for (auto& prop : source)
		{
			if (target.find(prop.first) == target.end())
			{
				target.add(prop.first, nullptr, InsertMethod::INSERT);
			}
		}
	}

	template<>
	inline EvaluationUnit*& correspond<PropertyAssignmentList, EvaluatedScope>(const Expression* expr, const PropertyAssignmentList& source, EvaluatedScope& target)
	{
		for (auto& prop : source)
		{
			ENFORCE_POINTER(prop->value);
			if (prop->value == expr)
			{
				return target[prop->propertyName];
			}
		}

		THROW("");
	}

	template<>
	inline EvaluationUnit*& correspond<ExpressionScope, EvaluatedScope>(const Expression* expr, const ExpressionScope& source, EvaluatedScope& target)
	{
		for (auto& prop : source)
		{
			ENFORCE_POINTER(prop.second);
			if (prop.second == expr)
			{
				return target[prop.first];
			}
		}

		THROW("");
	}
}