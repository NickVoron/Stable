#pragma once
#include "expression.h"

namespace Expressions 
{	
	Expression* addExpression(Expression* expr);
	EvaluationUnit* addExpression(EvaluationUnit* expr);

	template<class Expr, class... P0> Expr* add(P0&&... p0)
	{ 
		return (Expr*)addExpression(new Expr(std::forward<P0>(p0)...));
	}

	void collect_garbage();
	void evaluate();
}