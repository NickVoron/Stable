#include "callable.h"
#include "lambda.h"
#include "function.h"
#include "reference.h"
#include "functions/func.h"

namespace Expressions
{
	Callable::Callable(const std::string& name_, const ConstExprList& params_):
		  name(name_)
		, params(params_)
	{

	}

	void Callable::resolve(Type type, const Reference* lambdaRef)
	{
		switch (type)
		{
		case CALLABLE_FUNCTION_CALL:	resolved = add<Function>(name, params); break;
		case CALLABLE_STRUCT:			resolved = add<Struct>(name, params); break;
		case CALLABLE_LAMBDA_CALL:		resolved = add<LambdaCall>(name, params, lambdaRef); break;
		}

		ENFORCE_POINTER(resolved);
	}

	EvaluationUnit* Callable::evaluated(EvaluatedScope& namescope) const
	{
		ENFORCE_POINTER(resolved);
		return resolved->evaluated(namescope);
	}
}