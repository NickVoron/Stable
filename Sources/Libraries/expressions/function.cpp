// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "function.h"
#include "evaluation.h"
#include "functions/func.h"
#include <string>
#include <iterator>

namespace Expressions {

	Function::Function(const std::string& functionName_, const ConstExprList& params_) :
		functionName(functionName_), params(params_)
	{
	}


	EvaluationUnit* Function::evaluated(EvaluatedScope& environment) const
	{
		return Expressions::add<FunctionUnit>(environment, *this);
	}

	std::string Function::string() const
	{
		return str::spaced("function call:", functionName, params.string());
	}

	
	FunctionUnit::FunctionUnit(EvaluatedScope& parentScope_, const Function& function_) :
		EvaluationUnit("function_unit", function_, parentScope_),
		function(function_)
	{
		prepareEvaluationUnits(parentScope_, function.params, readyParams, false);
	}

	std::string FunctionUnit::string() const
	{
		return str::spaced("function unit:", function.functionName, readyParams.string());
	}


	EvaluationInfo FunctionUnit::evaluate()
	{
		bool completed = std::all_of(readyParams.begin(), readyParams.end(), [](auto& unit)
		{
			return unit->evaluate() == Complete;
		});

		EvaluationInfo result(Impossible);
		if (completed)
		{
			result.reject(FUNCTIONS::execute(function.functionName, readyParams));
		}
		return result;
	}

	void FunctionUnit::extract(EvaluationSet& result)
	{
		readyParams.extract(result);
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