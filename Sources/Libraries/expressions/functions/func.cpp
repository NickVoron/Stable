#include "func.h"
#include "arrayfunc.h"
#include "debug.h"

namespace Expressions
{

void FUNCTIONS::init()
{
	CommonFunctions::Functions::init();
	Expressions::Functions::array();
	Expressions::Functions::debug();
}

std::string EvaluationUnitTypeName::name(const EvaluationUnit* expr)
{
	if (expr)
	{
		return expr->typeName();	
	}

	return "ERROR";
}

}//



