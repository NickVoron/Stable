#include "002.componentLink.h"
#include <map>
#include <vector>
#include <memory>

#ifdef ENABLE_TEST


namespace ComponentModelTesting
{
using namespace Expressions;
using namespace ObjectParser;

//
ComponentLink::ComponentLink()
{
	std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/002.componentLink.desc");

	//parser test
	ObjectParser::Compiler comp(path.c_str());

	testClassesCount(comp.result, 2);
	
	ObjectParser::ClassDesc* mainClass = comp.result.classes().get("Main");
	ENFORCE_MSG(mainClass, "No 'Main' Class");
	{
		const InstanceDefinitionExpression* objectInstance = findInstance(mainClass, "object");
		ENFORCE_MSG(objectInstance, "No instance 'object' in 'Main' class");
		ENFORCE_MSG(objectInstance->params.size()== 0, "");
	}

	ObjectParser::ClassDesc* objectClass = comp.result.classes().get("Object");
	ENFORCE_MSG(objectClass, "No 'Object' class");
	{
		const Component* unitSpawner = testComponent(objectClass, "unitSpawner");
		testType(unitSpawner, "Spawner");

		const Component* buildingSpawner = testComponent(objectClass, "buildingSpawner");
		testType(buildingSpawner, "Spawner");

		const Component* logic = testComponent(objectClass, "logic");
		testType(logic, "Logic");

		ENFORCE_MSG(logic->linkParams.size() == 2, "");
	}

	auto mainScopename = unroll(comp.result.classes(), "Main", "main");
	
	const ComponentHandle* unitSpawner = get(mainScopename, "main.object.unitSpawner")->cast<ComponentHandle>();
	const ComponentHandle* buildingSpawner = get(mainScopename, "main.object.buildingSpawner")->cast<ComponentHandle>();
	const ComponentHandle* logic = get(mainScopename, "main.object.logic")->cast<ComponentHandle>();

	ENFORCE_MSG(logic->links.at("firstSpawner") == unitSpawner, "Link to 'firstSpawner' failed" );
	ENFORCE_MSG(logic->links.at("secondSpawner") == buildingSpawner, "Link to 'secondSpawner' failed");
}

}//

#endif