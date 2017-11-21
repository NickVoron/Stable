#include "014.componentByType.h"


#ifdef ENABLE_TEST
namespace ComponentModelTesting
{
	using namespace Expressions;
	using namespace ObjectParser;

	ComponentByType::ComponentByType()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/014.componentByType.desc");

		//parser test
		ObjectParser::Compiler comp(path.c_str());

		testClassesCount(comp.result, 3);
		
		auto worldScopename = unroll(comp.result.classes(), "Main", "main");

		auto objects = get(worldScopename, "main.object")->cast<ArrayContainer>();
		auto childPositionsCandidate = get(worldScopename, "main.manager.draw.childs");
		auto childPositions = childPositionsCandidate->cast<ArrayContainer>();

		ENFORCE_MSG(objects->count() == childPositions->count(), "");
		for (std::size_t i = 0; i < objects->count(); i++)
		{
			const EvaluationUnit* object = objects->element(i);
			//Expressions::PropertyPath propertyPath("position");
// 			auto objectPosition = object->child(&propertyPath);
// 			auto childPosition = childPositions->element(i);
// 		
// 			ENFORCE_EQUAL(objectPosition, childPosition);
		}
	}
}//
#endif
