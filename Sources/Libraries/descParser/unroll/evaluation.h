// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "../classDescEtc/propertyAssignment.h"

#include "expressions/library.include.h"

namespace Expressions
{
	using namespace ObjectParser;
	
	
	
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



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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