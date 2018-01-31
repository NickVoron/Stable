// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "lambda.h"
#include "reference.h"
#include "evaluation.h"

namespace Expressions
{

Lambda::Lambda()
{

}

Lambda::Lambda(const Expression* expr_):
	expr(expr_)
{

}

EvaluationUnit* Lambda::evaluated(EvaluatedScope& namescope) const 
{
	return Expressions::add<LambdaUnit>(namescope, *this);
}

void Lambda::assignParams(const ConstExprList& params_)
{
	callParams = params_;
}

void Lambda::assignCapture(const ConstExprList& captureParams_)
{
	captureParams = captureParams_;
}

const Expression* Lambda::internalExpression() const
{
	return expr;
}

const ConstExprList& Lambda::capture() const
{
	return captureParams;
}

const ConstExprList& Lambda::parameters() const
{
	return callParams;
}




LambdaUnit::LambdaUnit(EvaluatedScope& namescope, const Lambda& proto) : 
	EvaluationUnit("lanbda_unit", proto, namescope)
	, prototype(proto)	
{

}

const Lambda& LambdaUnit::master() const
{
	return prototype;
}










LambdaCall::LambdaCall(const std::string& name_, const ConstExprList& params_, const Reference* lambda_) :
	name(name_),
	params(params_),
	lambda(lambda_)

{

}

EvaluationUnit* LambdaCall::evaluated(EvaluatedScope& namescope) const
{
	auto callUnit = Expressions::add<LambdaCallUnit>(namescope, *this);
	callUnit->lambdaUnit = lambda->evaluated(namescope);	
	return callUnit;
}

std::string LambdaCall::string() const
{
	return str::spaced("lambda call:", name, params.string());
}




LambdaCallUnit::LambdaCallUnit(EvaluatedScope& parent, const LambdaCall& proto) :
	EvaluationUnit("lanbda_call_unit", proto, parent),
	prototype(proto)
{
	prepareEvaluationUnits(scope(), prototype.params, callParams, false);
}

EvaluationInfo LambdaCallUnit::evaluate()
{
	EvaluationInfo result(Impossible);

	if(lambdaUnit->evaluate() == Complete)
	{
		if(const auto unit = lambdaUnit->cast<LambdaUnit>())
		{
			const auto internalExpr = unit->master().internalExpression();
			resultUnit = internalExpr->evaluated(scope());

			auto prototypeParams = unit->master().parameters();
			ENFORCE_EQUAL(callParams.size(), prototypeParams.size());

			for (std::size_t i = 0; i < callParams.size(); ++i)
			{
				auto& lparam = prototypeParams[i];
				auto& pparam = callParams[i];
				auto ref = lparam->cast<Reference>();
				ENFORCE_POINTER(ref);
				ENFORCE_EQUAL(ref->path.size(), 1);
				scope().add(ref->path.root(), pparam, InsertMethod::REPLACE_OR_INSERT);
			}
		}	
		
		bool completed = std::all_of(callParams.begin(), callParams.end(), [](auto& unit)
		{
			return unit->evaluate() == Complete;
		});

		completed &= std::all_of(scope().begin(), scope().end(), [](auto& unit)
		{
			return unit.second->evaluate() == Complete;
		});

		if(completed)
		{
			result.reject(resultUnit);
		}
	}	
	else
	{
		result.reject(this);
	}
	
	return result;
}

void LambdaCallUnit::extract(EvaluationSet& result)
{
	if(lambdaUnit) result.add(&lambdaUnit);
	if(resultUnit) result.add(&resultUnit);

	callParams.extract(result);
	EvaluationUnit::extract(result);
}
																			

}//



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