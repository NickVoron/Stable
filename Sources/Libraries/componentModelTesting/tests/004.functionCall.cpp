#include "004.functionCall.h"

#ifdef ENABLE_TEST

namespace ComponentModelTesting
{

using namespace Expressions;
using namespace ObjectParser;

Vector3 getPosition()
{
	return Vector3(1, 2, 3);
}


FunctionCall::FunctionCall()
{
	BIND_EXPRESSION_FUNCTION(getPosition);
	
	std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/004.fuctionCall.desc");

	//parser test
	ObjectParser::Compiler comp(path.c_str());

	testClassesCount(comp.result, 2);

	auto mainScopename = unroll(comp.result.classes(), "Main", "main");
	auto unrolledMainInstace = testInstance(mainScopename, "main");

	ObjectParser::ClassDesc* mainClass = comp.result.classes().get("Main");
	ENFORCE_MSG(mainClass, "No 'Main' Class");

	{
		const InstanceDefinitionExpression* objectInstance = findInstance(mainClass, "object");
		ENFORCE_MSG(objectInstance, "No instance 'object' in 'Main' class");
		ENFORCE_MSG(objectInstance->params.size() == 1, "");
	}

	//main.object instcance
	auto unrolledObjectExpr = unrolledMainInstace->scope().get("object");
	ENFORCE_MSG(unrolledObjectExpr, "No 'object' expression in main instance");
	const ObjectParser::InstanceHandle* unrolledObjectInstace = unrolledObjectExpr->cast<const ObjectParser::InstanceHandle>();
	ENFORCE_MSG(unrolledObjectInstace, "'object' expression in not ObjectParser::InstanceHandle");

	auto positionExpr = unrolledObjectInstace->scope().get("position");
	auto position = positionExpr->cast<Expressions::EvalStruct>();

	ENFORCE_POINTER(position);
	ENFORCE_EQUAL(position->typeName(), "Vector3");
	
	test(*position, Vector3(1, 2, 3));
}

}//

#endif