#include "006.dataStruct.h"

#ifdef ENABLE_TEST

namespace ComponentModelTesting
{

	using namespace Expressions;
	using namespace ObjectParser;

	DataStruct::DataStruct()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/006.dataStruct.desc");

		//parser test
		ObjectParser::Compiler comp(path.c_str());
		
		testClassesCount(comp.result, 1);
		auto worldScopename = unroll(comp.result.classes(), "Main", "main");
		testInstance(worldScopename, "main");
		test(worldScopename, "main.myStruct.index", 10);
	}

}//

#endif