#include "015.nestedObject.h"

#ifdef ENABLE_TEST
namespace ComponentModelTesting
{
	using namespace Expressions;
	using namespace ObjectParser;

	NestedObject::NestedObject()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/015.nestedObject.desc");

		//parser test
		ObjectParser::Compiler comp(path.c_str());

		testClassesCount(comp.result, 3);
		
		auto worldScopename = unroll(comp.result.classes(), "Main", "main");
		testInstance(worldScopename, "main");		

		for (int i = 0; i < 10; ++i)
		{
			test(worldScopename, str::stringize("main.object.debug[", i, "].debugCount"), i);
		}
	}

}//
#endif