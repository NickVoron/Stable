#include "005.dataArray.h"

#ifdef ENABLE_TEST

namespace ComponentModelTesting
{
	using namespace Expressions;
	using namespace ObjectParser;

	DataArray::DataArray()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/005.dataArray.desc");

		//parser test
		ObjectParser::Compiler comp(path.c_str());

		testClassesCount(comp.result, 1);
		
		auto worldScopename = unroll(comp.result.classes(), "Main", "main");
		testInstance(worldScopename, "main");		

		for (int i = 4; i >= 0; --i)
		{
			test(worldScopename, str::stringize("main.myArray[", i, "]"), i);
		}
	}

}//

#endif