// Copyright (C) 2014-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "function.h"
#include "functions/func.h"
#include <string>
#include <iterator>

namespace Expressions{

Function::Function(const std::string& functionName_, const ConstExprList& params_) :
		functionName(functionName_), params(params_)
{
}


EvaluationUnit* Function::evaluated(const EvaluatedScope& environment) const
{
	ConstExprList evaluatedParams;
	params.evaluate(environment, evaluatedParams);


	auto result = FUNCTIONS::execute(functionName, evaluatedParams);
	auto evaluated = result->cast<EvaluationUnit>();
	ENFORCE_POINTER(evaluated);
	return evaluated;
}

std::string Function::string() const
{
	return str::spaced("function call:", functionName, params.string());
}


FunctionUnit::FunctionUnit(const EvaluatedScope& parent, const std::string& functionName_, const ConstExprList& params_) :
	EvaluationUnit(parent),
	functionName(functionName_), params(params_)
{

}

std::string FunctionUnit::string() const
{
		return str::spaced("function unit:", functionName, params.string());
}

Expressions::EvaluateState FunctionUnit::urollParams(const Expressions::EvaluatedScope& parentScopename)
{
	EvaluateState evalState = Complete;

	if (params.size())
	{
		evalState = Expressions::Impossible;  

		params.erase(std::remove_if(params.begin(), params.end(), [this, &parentScopename, &evalState](auto& param)
		{
			int index = std::distance(params.begin(), std::find(params.begin(), params.end(), param));
			const std::string& name = std::to_string(index);
			if (param->references().canResolveReference(parentScopename))
			{
				EvaluationUnit* evalUnit = param->evaluated(parentScopename);
				add(name, evalUnit, InsertMethod::INSERT);
				evalState = Reject;	

				return true;
			}

			return false;

		}), params.end());
	}

	return evalState;
}


Expressions::EvaluateState FunctionUnit::evaluateStep(const EvaluatedScope& parentScopename)
{
	if (isParent(parentScopename))
	{
		
		EvaluateState result = urollParams(parentScopename);

		
		for (auto& iter : *this)
		{
			EvaluationUnit* unit = iter.second;
			if (isClassMember(unit))
			{
				EvaluateState unitState = unit->evaluateStep(*this);
				result = merge(result, unitState);

				if (result == Impossible)
				{
					volatile int i = 0;
				}

			}
		}
		return result;
	}

	return Expressions::Complete;
}

}




// Copyright (C) 2014-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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