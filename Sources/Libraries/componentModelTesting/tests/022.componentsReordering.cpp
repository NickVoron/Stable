#include "022.componentsReordering.h"

#include "utils.h"


#ifdef ENABLE_TEST
namespace ComponentModelTesting
{
	ComponentsReordering::ComponentsReordering()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/022.componentsReordering.desc");
		

		ObjectParser::Compiler comp(path.c_str());
		auto worldScopename = unroll(comp.result.classes(), "Main", "main");

		auto utilizer0_Blue = get(worldScopename, "main.utilizer0.blue");
		auto utilizer0_Red = get(worldScopename, "main.utilizer0.red");

		test(worldScopename, "main.utilizer0.utilizer.references[0]", utilizer0_Red);
		test(worldScopename, "main.utilizer0.utilizer.references[1]", utilizer0_Blue);

		//reoderd 
		auto utilizer1_Blue = get(worldScopename, "main.utilizer1.blue");
		auto utilizer1_Red = get(worldScopename, "main.utilizer1.red");

		test(worldScopename, "main.utilizer1.utilizer.references[0]", utilizer1_Blue);
		test(worldScopename, "main.utilizer1.utilizer.references[1]", utilizer1_Red);


		ComponentModel::descriptionTest(path.c_str());
	}
}
#endif