#include "016.lambda.h"

#ifdef ENABLE_TEST
namespace ComponentModelTesting
{
	Lambda::Lambda()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/016.lambda.desc");

		ObjectParser::Compiler comp(path.c_str());
		auto world = unroll(comp.result.classes(), "Main", "main");
		testInstance(world, "main");

// 		test(world, "main.val0", 3);
//   		test(world, "main.val1", 7 );
//   		test(world, "main.val2", 7);
// 		test(world, "main.val3", 2 );
// 		test(world, "main.val4", 5 );
// 		test(world, "main.val5", 9 );
	}
}//
#endif