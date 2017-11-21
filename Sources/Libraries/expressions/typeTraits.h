#pragma once

#include "expression.h"

namespace Expressions
{
	template<class ConstType> 
	struct VarToType
	{
		typedef ConstType ExprType;
		typedef typename ConstType::Type Type;
		static const char* name() { return typeid(Type).name(); }
	};
}