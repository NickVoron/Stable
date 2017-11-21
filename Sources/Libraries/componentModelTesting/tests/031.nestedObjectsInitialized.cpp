#include "031.nestedObjectsInitialized.h"
#include "commonComponents/library.include.h"
#include "utils.h"

#ifdef ENABLE_TEST
namespace ComponentModelTesting
{

	NestedObjectsInitialized::NestedObjectsInitialized()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/030.nestedObjectsInitialized.desc");
		ObjectParser::Compiler comp(path.c_str());
		auto worldScopename = unroll(comp.result.classes(), "Main", "main");

		auto mixes = get(worldScopename, "main.object.mixes");
		ENFORCE_POINTER(mixes);

	}

}//
#endif