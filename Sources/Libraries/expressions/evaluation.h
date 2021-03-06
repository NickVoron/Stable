// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "expression.h"
#include "constExprList.h"


namespace Expressions
{
	template<class SourceHolder, class TargetHolder>
	void preparePlaceholder(const SourceHolder& source, TargetHolder& target);

	template<class SourceHolder, class TargetHolder>
	EvaluationUnit*& correspond(const Expression* expr, const SourceHolder& source, TargetHolder& target);

	template<class UnitType>
	EvaluationUnit*& unit(UnitType& value);

	template<class UnitType>
	std::string unit_name(UnitType& value);

	template<class UnitType>
	const Expression* extrexpr(UnitType& value)
	{
		return value;
	}

	template<class Key>
	inline const Expression* extrexpr(const std::pair<Key, const Expression*>& value)
	{
		return value.second;
	}

	
	
	
	template<>
	inline EvaluationUnit*& unit(EvaluationUnit*& value)
	{
		return value;
	}

	template<>
	inline std::string unit_name(EvaluationUnit*& value)
	{
		return "";
	}

	template<class Key>
	inline EvaluationUnit*& unit(std::pair<Key, EvaluationUnit*>& value)
	{
		return value.second;
	}

	template<class Key>
	inline std::string unit_name(std::pair<Key, EvaluationUnit*>& value)
	{
		return str::stringize(value.first);
	}

	template<>
	inline void preparePlaceholder<ConstExprList, EvaluationUnitsList>(const ConstExprList& source, EvaluationUnitsList& target)
	{
		if (target.empty() && !source.empty())
		{
			target.resize(source.size());
		}		
		ENFORCE_EQUAL(source.size(), target.size());
	}

	template<>
	inline EvaluationUnit*& correspond<ConstExprList, EvaluationUnitsList>(const Expression* src, const ConstExprList& source, EvaluationUnitsList& target)
	{
		auto it = std::find(source.begin(), source.end(), src);
		auto idx = std::distance(source.begin(), it);
		ENFORCE((decltype(idx)) target.size() > idx);
		return target[idx];
	}

	template<class T0, class T1>
	class StaticTest
	{
		static_assert(std::is_same_v<T0, T1>);
	};

	template<class SourceHolder, class TargetHolder>
	void prepareEvaluationUnits(Expressions::EvaluatedScope& namescope, const SourceHolder& source, TargetHolder& target, bool classMembers)
	{
		if (!source.empty())
		{
			preparePlaceholder(source, target);

			for (auto& src : source)
			{
				const Expression* expr = extrexpr(src);
				EvaluationUnit*& unit = correspond(expr, source, target);
				
				{
					unit = expr->evaluated(namescope);
					if (classMembers)
					{
						target.classMembers.insert(unit);
					}
				}
			}
		}
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