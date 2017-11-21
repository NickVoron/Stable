#include "011.spawner.h"
#include "utils.h"
#include "utils.h"


#ifdef ENABLE_TEST

namespace ComponentModelTesting
{
	using namespace Expressions;
	using namespace ObjectParser;

	Spawner::Spawner()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/011.spawner.desc");

// 		ObjectParser::Compiler comp(path.c_str());
// 		auto worldScopename = unroll(comp.result.classes(), "Main", "main");
		
		// убедимся что прототип есть и он инстанциировал свой параметр
		//test(worldScopename, "main.objectsSpawner.spawner.prototype.one", 1);
		//test(worldScopename, "main.objectsSpawner.spawner.prototype.one", 1);

		ComponentModel::descriptionTest(path.c_str());
	}
}

#endif