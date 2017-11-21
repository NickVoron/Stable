#pragma once

#include "expressions/library.include.h"
#include "stuff/library.include.h"
#include "TGFParser/library.include.h"

namespace ExpressionFunctions
{
	std::vector<std::vector<std::string>> load_ods(const std::string& fileName);
}