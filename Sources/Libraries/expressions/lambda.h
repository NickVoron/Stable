#pragma once
#include "expression.h"
#include "constExprList.h"
#include "namescope.h"

namespace Expressions
{
	class Lambda : public Expression
	{
	public:
		Lambda();
		Lambda(const Expression* exrp);		

		virtual EvaluationUnit* evaluated(EvaluatedScope& namescope) const override;

		void assignParams(const ConstExprList& params);
		void assignCapture(const ConstExprList& captureParams);

		const Expression* internalExpression() const;
		const ConstExprList& capture() const;
		const ConstExprList& parameters() const;

	protected:
		const Expression* expr;
		ConstExprList captureParams;
		ConstExprList callParams;
	};

	class LambdaUnit : public EvaluationUnit
	{
	public:
		LambdaUnit(EvaluatedScope& namescope, const Lambda& prototype);
		const Lambda& master() const;

	protected:
		const Lambda& prototype;
	};

	//
	class LambdaCall : public Expression
	{
	public:
		LambdaCall(const std::string& name, const ConstExprList& params, const Reference* lambda);

		virtual EvaluationUnit* evaluated(EvaluatedScope& environment) const override;
		virtual std::string string() const override;
		virtual std::string typeName() const override { return str::spaced("lambda call:", name); }

		const Reference* lambda;
		const std::string name;
		ConstExprList params;
	};

	//
	class LambdaCallUnit : public EvaluationUnit
	{
	public:
		LambdaCallUnit(EvaluatedScope& parent, const LambdaCall& proto);

		virtual std::string string() const override   { return str::stringize(typeName(), callParams.string()); }
		virtual std::string typeName() const override { return str::spaced("lambda call unit:", prototype.name); }

		virtual EvaluationInfo evaluate();
		virtual void extract(EvaluationSet& result) override;

		const LambdaCall& prototype;
		EvaluationUnitsList callParams;					
		EvaluationUnit* resultUnit = nullptr;
		EvaluationUnit* lambdaUnit = nullptr;
	};
}
