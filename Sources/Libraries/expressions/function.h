#pragma once
#include "expression.h"
#include "constExprList.h"
#include "namescope.h"

namespace Expressions
{
	class Function : public Expression
	{
	public:
		Function(const std::string& functionName, const ConstExprList& params);

		virtual EvaluationUnit* evaluated(EvaluatedScope& environment) const override;
		virtual std::string string() const override;
		virtual std::string typeName() const override { return str::spaced("function:", functionName); }

		ConstExprList params;
		const std::string functionName;
	};

	//FunctionUnit
	class FunctionUnit : public EvaluationUnit
	{
	public:
		FunctionUnit(EvaluatedScope& parent, const Function& function);

		virtual std::string string() const override;
		virtual std::string typeName() const override { return str::spaced("function:", function.functionName); }
		virtual EvaluationInfo evaluate() override;
		virtual void extract(EvaluationSet& result) override;

		const Function& function;
		EvaluationUnitsList readyParams;
	};

}//