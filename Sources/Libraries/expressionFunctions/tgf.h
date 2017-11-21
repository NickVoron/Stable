#pragma once

#include "expressions/library.include.h"
#include "stuff/library.include.h"
#include "TGFParser/library.include.h"

namespace ExpressionFunctions
{
	tgf::TGFGraph load_tgf(const std::string& fileName);
	Vector3 node_pos(int count);
}