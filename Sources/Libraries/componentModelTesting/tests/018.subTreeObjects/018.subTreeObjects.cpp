#include "018.subTreeObjects.h"
#include "types.h"

#ifdef ENABLE_TEST
namespace ComponentModelTesting
{
	using namespace Expressions;
	using namespace ObjectParser;

	SubTreeObjects::SubTreeObjects()
	{
		//BIND_EXPRESSION_FUNCTION(parseTree);

		//std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/018.subTreeObjects.desc");
		////parser test
		//ObjectParser::Compiler comp(path.c_str());

		//Expressions::EvaluatedScope worldScopename = unroll(comp.result.classes(), "Main", "main");
		//testInstance(worldScopename, "main");

		//for (int i = 0; i < 10; i++)
		//{
		//	const Expression* object = get(worldScopename, str::stringize("main.objects[", i, "].subObjects"));
		//	const EvaluatedArray* objectInstance = object->cast<EvaluatedArray>();
		//	ENFORCE_MSG(objectInstance->count() == i, "");

		//	for (int j = 0; j < i; j++)
		//	{
		//		const Expression* subObject = get(worldScopename, str::stringize("main.objects[",i,"].subObjects[",j,"]"));
		//		test(worldScopename, str::stringize("main.objects[", i, "].subObjects[", j, "].index"), j);
		//	}
		//}
	}

}//
#endif