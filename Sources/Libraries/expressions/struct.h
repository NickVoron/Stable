#pragma once
#include "holder.h"
#include "constExprList.h"

namespace Expressions
{
	//Struct
	class Struct : public Expression
	{
	public:
		Struct(const std::string& structType, const ConstExprList& params);

 		virtual EvaluationUnit* evaluated(EvaluatedScope& environment) const override;
 		virtual std::string string() const override;
		
		const std::string structType;
		const ConstExprList params;
	};

	class EvalStruct : public EvaluationUnit
	{
	public:
		EvalStruct(EvaluatedScope& scope, const Struct& prototype);

		virtual std::string typeName() const override { return prototype.structType; }
		virtual std::string string() const override { return str::stringize(typeName(), params.string()); }
		virtual void extract(EvaluationSet& result) override;

		const Struct& prototype;
		EvaluationUnitsList params;
	};
}
