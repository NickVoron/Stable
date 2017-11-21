#pragma once
#include "expression.h"
#include "constExprList.h"

namespace Expressions
{
	class Callable : public Expression
	{
	public:

		enum Type
		{
			CALLABLE_FUNCTION_CALL,
			CALLABLE_STRUCT,
			CALLABLE_LAMBDA_CALL,
			CALLABLE_UNRESOLVED_TYPE
		};

		Callable(const std::string& name, const ConstExprList& params);

		virtual EvaluationUnit* evaluated(EvaluatedScope& namescope) const override;
		virtual std::string typeName() const override { return name; }
		virtual std::string string() const override { return str::stringize(typeName(), params.string()); }
		
		void resolve(Type type, const Reference* lambdaRef);

		std::string name;
		ConstExprList params;
		const Expression* resolved = nullptr;
	};
}
