#include "binder.h"
#include "holder.h"

namespace Expressions
{

	//binder

	BinderExpression::BinderExpression(EvaluatedScope& scopeName_, const Reference* proxy_):
		scopeName(scopeName_),
		proxy(proxy_)
	{

	}

	EvaluationUnit* BinderExpression::evaluated(EvaluatedScope& environment) const
	{
		//if (real)
		//{
		//	return real->evaluated(environment, userData);
		//}

		//return this;

		return 0;
	}


	void BinderExpression::set(const Expression* real_) const
	{	
		real = real_;
	}


	//
	BinderExpression* bind(EvaluatedScope& scopeName, const Reference* proxy)
	{
		return add<BinderExpression>(scopeName, proxy);
	}
}