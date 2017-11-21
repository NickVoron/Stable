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

//
//
//
LambdaUnit::LambdaUnit(EvaluatedScope& namescope, const Lambda& proto) : 
	EvaluationUnit("lanbda_unit", proto, namescope)
	, prototype(proto)	
{

}

const Lambda& LambdaUnit::master() const
{
	return prototype;
}

// EvaluationInfo LambdaUnit::evaluate()
// {
// 	ENFORCE_POINTER(result);
// 	return result->evaluateStep(namescope);
// }

//
//
//
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

//
//
//
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