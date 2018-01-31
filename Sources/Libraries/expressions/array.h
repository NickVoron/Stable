// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "expression.h"
#include "constExprList.h"
#include "namescope.h"

namespace Expressions
{
	template<class T>
	bool convertVar(const EvaluationUnit& var, T& value);

	
	class Array : public Expression
	{
	public:
		Array(){}
		Array(const ConstExprList& params);

		virtual EvaluationUnit* evaluated(EvaluatedScope& environment) const override;
		virtual std::string string() const override;
		virtual std::string typeName() const override;
	
		void add(const Expression* expr);
		const Expression* element(std::size_t index) const;
		std::size_t count() const;

		ConstExprList elements;
	};

	class ArrayContainer : public EvaluationUnit
	{
	public:
		ArrayContainer(const Expression& proto, EvaluatedScope& parent);
		ArrayContainer(const Expression& proto, EvaluatedScope& parent, EvaluationUnitsList& elements);

		virtual std::string string() const override;
		virtual std::string typeName() const override;

		virtual EvaluationInfo evaluate() override;
		virtual void extract(EvaluationSet& result) override;

		EvaluationUnit*& add(EvaluationUnit* expr);

		const EvaluationUnit* element(std::size_t index) const;
		EvaluationUnit* element(std::size_t index);
		std::size_t count() const;

		EvaluationUnitsList elements;
	};


	template<class TargetType>
	void linearize(EvaluationUnitsList& elements, std::vector<TargetType>& result);

	template<class TargetType>
	void linearize(EvaluationUnit& expr, std::vector<TargetType>& result)
	{
		if (auto units = expr.cast<ArrayContainer>())
		{
			linearize(units->elements, result);
		}
		else if (auto unit = expr.cast<EvaluationUnit>())
		{
			TargetType type;
			if(convertVar(*unit, type))
			{
				result.push_back(type);
			}			
		}
		else
		{
			THROW("expression must be requested unit type or Array of them");
		}
	}

	template<class TargetType>
	void linearize(EvaluationUnitsList& elements, std::vector<TargetType>& result)
	{
		for (std::size_t i = 0; i < elements.size(); ++i)
		{
			linearize(*elements[i], result);
		}
	}

	ArrayContainer* to_array(EvaluationUnit* unit);
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