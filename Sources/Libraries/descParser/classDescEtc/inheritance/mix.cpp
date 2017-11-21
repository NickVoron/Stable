#include "mix.h"

namespace ObjectParser
{

	std::string MixInheritance::string() const
	{
		return str::stringize("ParentInit type = ", type).str();
	}


	Expressions::EvaluationUnit* MixInheritance::evaluated(Expressions::EvaluatedScope& environment) const
	{
		INCOMPLETE;
		return 0;
	}

}//