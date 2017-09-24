// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "constExprList.h"
#include "expression.h"
#include "reference.h"

namespace Expressions
{

	ExprList::ExprList(const ExprList& exprList):
		std::vector<Expression*>(exprList)

	{


	}

	
	ConstExprList::ConstExprList(const ExprList& exprList)
	{
		reserve(exprList.size());
		for (const Expression* expr : exprList)
		{
			push_back(expr);
		}
	}	

	void ConstExprList::evaluate(const EvaluatedScope& environment, ConstExprList& result) const
	{
		
		std::set<EvaluationUnit*> members;

		result.resize(size());
		for (std::size_t i = 0; i < size(); ++i)
		{
			const Expression* param = at(i);
			EvaluationUnit* paramUnit = param->evaluated(environment);
			result[i] = paramUnit;
			if (!param->cast<Reference>())
			{
				members.emplace(paramUnit);
			}
		}

		EvaluateState evalState = Reject;
		while (evalState != Complete)
		{
			evalState = Complete;
			for(auto& paramUnit: members)
			{
				evalState = paramUnit->evaluateStep(environment);
			}

			ENFORCE(evalState != Impossible);
		}
	}

	std::string ConstExprList::string() const
	{
		return str::stringize("(", str::comma(), naked_string(), str::nodelim(), ")");
	}

	std::string ConstExprList::naked_string() const
	{
		str::stringize result;
		for (const Expression* expr : *this)
		{
			result(stringize(expr));
		}
		return result;
	}

	References ConstExprList::references() const
	{
		References refs;
		for (const Expression* expr : *this)
		{
			auto exprRefs = expr->references();
			refs.insert(refs.begin(), exprRefs.begin(), exprRefs.end());
		}

		return refs;
	}


EvalUnitList::EvalUnitList(const EvalUnitList& exprList)
{
	reserve(exprList.size());
	for (auto expr : exprList)
	{
		push_back(expr);
	}
}

std::string EvalUnitList::string() const
{
	str::stringize result("(", str::comma());
	for (const Expression* expr : *this)
	{
		result(stringize(expr));
	}
	result(str::nodelim(), ")");

	return result;
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