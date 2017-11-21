#pragma once
#include "expressions/library.include.h"
#include "componentModel/library.include.h"

namespace Expressions
{
	bool convert(Expressions::EvaluationUnit& expr, LinksDescList& client);
	bool convert(Expressions::EvaluationUnit& expr, LinkDesc& client);
	bool convert(Expressions::EvaluationUnit& expr, ValueAddress& client);
}