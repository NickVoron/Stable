#include "008.objectArray.h"


#ifdef ENABLE_TEST

namespace ComponentModelTesting
{

	using namespace Expressions;
	using namespace ObjectParser;

	ObjectArray::ObjectArray()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/008.objectArray.desc");

		//parser test
		ObjectParser::Compiler comp(path.c_str());

		testClassesCount(comp.result, 2);

		auto worldScopename = unroll(comp.result.classes(), "Main", "main");
		
		for (int i = 0; i < 10; ++i)
		{
			std::string path = str::stringize("main.objectIntArray[", i, "].index");
			test(worldScopename, path, i);
		}


		test(worldScopename, "main.objectComplexArray[0].text", std::string("first"));
		test(worldScopename, "main.objectComplexArray[1].text", std::string("second"));
		test(worldScopename, "main.objectComplexArray[2].text", std::string("third"));
	}
}

#endif