#include "007.complexData.h"

#ifdef ENABLE_TEST

namespace ComponentModelTesting
{

	using namespace Expressions;
	using namespace ObjectParser;

	ComplexData::ComplexData()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/007.complexData.desc");

		//parser test
		ObjectParser::Compiler comp(path.c_str());

		testClassesCount(comp.result, 1);

		auto worldScopename = unroll(comp.result.classes(), "Main", "main");
				
		for(int i=0; i < 3; ++i)
		{
			std::string indexPath = str::stringize("main.complexData[", i, "].index");
			test(worldScopename, indexPath, i);
		}

		test(worldScopename, "main.complexData[0].text", std::string("first"));
		test(worldScopename, "main.complexData[1].text", std::string("second"));
		test(worldScopename, "main.complexData[2].text", std::string("third"));
	}
}//

#endif