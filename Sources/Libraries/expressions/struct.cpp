#include "struct.h"

namespace Expressions{

	//
	Struct::Struct(const std::string& structType_, const ConstExprList& params_) :
		structType(structType_),
		params(params_)
	{

	}

	std::string Struct::string() const
	{
		str::stringize result(typeName(), "(", str::comma());
		
		for(auto& pe : params)
		{
			result(stringize(pe));
		}

		return result(str::delimeter(), ")");
	}

	EvaluationUnit* Struct::evaluated(EvaluatedScope& environment) const
	{
		auto evaluatedValue = Expressions::add<EvalStruct>(environment, *this);
		evaluatedValue->params = params.evaluated(environment);
		return evaluatedValue;
	}


	EvalStruct::EvalStruct(EvaluatedScope& scope, const Struct& prototype_) :
		EvaluationUnit("eval_struct", prototype_, scope)
		, prototype(prototype_)
	{

	}

	void EvalStruct::extract(EvaluationSet& result)
	{
		params.extract(result);
	}

}//
