#pragma once
#include "expression.h"


namespace Expressions
{
	//binder
	class BinderExpression : public Expression
	{
	public:
		BinderExpression(EvaluatedScope& scopeName, const Reference* proxy);
		virtual EvaluationUnit* evaluated(EvaluatedScope& environment) const override;

		void set(const Expression* real) const;
	private:
		const EvaluatedScope& scopeName;
		const Reference* proxy;
		mutable const Expression* real = 0;
	};

	BinderExpression* bind(EvaluatedScope& scopeName, const Reference* proxy);
}