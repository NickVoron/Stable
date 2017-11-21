#pragma once
#include <memory>

namespace ComponentModelTesting
{
#define DEFINE_BUG_SCRIPT_COMPILATION(BugName) struct BugName \
	{\
		BugName()\
		{\
			std::string path = Resources::resourceRelativePath("desc/bugs/" STPP_STRINGIZE(BugName) ".desc");\
			ObjectParser::Compiler comp(path.c_str());\
			std::unique_ptr<Expressions::EvaluatedScope> worldScopename = ObjectParser::unroll(comp.result.classes(), "Main", "main");\
		}\
	};

	DEFINE_BUG_SCRIPT_COMPILATION(bug0008)
	DEFINE_BUG_SCRIPT_COMPILATION(bug0009)
	DEFINE_BUG_SCRIPT_COMPILATION(bug0011)
	DEFINE_BUG_SCRIPT_COMPILATION(bug0012)
}