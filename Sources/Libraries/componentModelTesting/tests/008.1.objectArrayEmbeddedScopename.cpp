#include "008.1.objectArrayEmbeddedScopename.h"

#ifdef ENABLE_TEST

namespace ComponentModelTesting
{

	using namespace Expressions;
	using namespace ObjectParser;

	ObjectArrayEmbeddedScopename::ObjectArrayEmbeddedScopename()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/008.1.objectArrayEmbeddedScopename.desc");

		//parser test
		ObjectParser::Compiler comp(path.c_str());

		testClassesCount(comp.result, 2);
		
		auto worldScopename = unroll(comp.result.classes(), "Main", "main");
		
		int constanta = get(worldScopename, "main.constanta")->cast<Const<int>>()->value;
		
		ENFORCE(constanta == 1);

		for (int i = 0; i < 10; ++i)
		{
			std::string path = str::stringize("main.objectIntArray[", i, "].index");
			test(worldScopename, path, constanta + i);
		}
	}
}

#endif