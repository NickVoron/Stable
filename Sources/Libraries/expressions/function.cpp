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

	//
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

}//
