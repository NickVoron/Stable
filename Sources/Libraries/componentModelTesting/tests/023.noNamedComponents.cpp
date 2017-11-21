#include "023.noNamedComponents.h"
#include "utils.h"


#ifdef ENABLE_TEST
namespace ComponentModelTesting
{
	NoNamedComponents::NoNamedComponents()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/023.noNamedComponents.desc");
 		ObjectParser::Compiler comp(path.c_str());
		

		std::unique_ptr<Expressions::EvaluatedScope> worldScopename = unroll(comp.result.classes(), "Main", "main");

		test(*worldScopename.get(), "main.server.Position_1.position", Vector3(20.0f, 0.0f, 30.0f));
		//ENFORCE(get(worldScopename, "main.server.SphereDrawer_2")!=0);

		//ComponentModel::descriptionTest(path.c_str());

	}
}
#endif