#include "003.componentAsParameter.h"

#ifdef ENABLE_TEST

namespace ComponentModelTesting
{

using namespace ObjectParser;

ComponentAsParameter::ComponentAsParameter()
{
	std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/003.componentAsParameter.desc");

	//parser test
	ObjectParser::Compiler comp(path.c_str());
	
	testClassesCount(comp.result, 2);

	
	auto worldScopename = unroll(comp.result.classes(), "Main", "main");

	const ComponentHandle* spawner = get(worldScopename, "main.spawner")->cast<ComponentHandle>();
	const ComponentHandle* buildingSpawner = get(worldScopename, "main.object.buildingSpawner")->cast<ComponentHandle>();

	test(worldScopename, "main.object.logic.firstSpawner", spawner);
	test(worldScopename, "main.object.logic.secondSpawner", buildingSpawner);
}



}//

#endif