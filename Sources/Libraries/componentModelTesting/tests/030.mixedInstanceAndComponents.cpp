#include "030.mixedInstanceAndComponents.h"

#include "utils.h"

#include "componentModel/library.include.h"
#include "commonComponents/library.include.h"

#ifdef ENABLE_TEST
namespace ComponentModelTesting
{

	MixedInstanceAndComponents::MixedInstanceAndComponents()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/030.mixedInstanceAndComponents.desc");
		ObjectParser::Compiler comp(path.c_str());
		auto worldScopename = unroll(comp.result.classes(), "Main", "main");

		auto mixes = get(worldScopename, "main.object.mixes");
		ENFORCE_POINTER(mixes);

	}

}//
#endif