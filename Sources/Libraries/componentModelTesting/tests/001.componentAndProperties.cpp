#include "001.componentAndProperties.h"
#include "utils.h"


#ifdef ENABLE_TEST

namespace ComponentModelTesting
{
	using namespace Expressions;
	using namespace ObjectParser;

	ComponentAndProperties::ComponentAndProperties()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/001.componentsAndProperties.desc");

		//parser test
		ObjectParser::Compiler comp(path.c_str());

		testClassesCount(comp.result, 2);

		const ObjectParser::ClassTable& table = comp.result.classes();
		
		//main class 
		ObjectParser::ClassDesc* mainClass = table.get("Main");
		ENFORCE_MSG(mainClass, "No 'Main' Class");
		{
			const InstanceDefinitionExpression* objectInstance = findInstance(mainClass, "object");
			ENFORCE_MSG(objectInstance, "No instance 'object' in 'Main' class");
			ENFORCE_MSG(objectInstance->params.size() == 3, "The number of parameters 'main.object' instance, does not match");

			ENFORCE_MSG(objectInstance->params[0]->propertyName == "pos", "The first param 'main.object' instance, does not 'pos'");
			ENFORCE_MSG(objectInstance->params[1]->propertyName == "vel", "The first param 'main.object' instance, does not 'vel'");
		}

		//object class 
		ObjectParser::ClassDesc* objectClass = table.get("Object");
		ENFORCE_MSG(objectClass, "No 'Object' class");
		{
			auto velExpr = objectClass->properties().get("vel");
			ENFORCE_MSG(velExpr, "No property 'vel' in class 'Object'");

			auto velStruct = velExpr->cast<Expressions::Callable>();
			ENFORCE_MSG(velStruct, "Property 'vel' is no type Struct");
			ENFORCE_MSG(velStruct->typeName() == "Vector3", "Struct 'vel' is no type Vector3");

			auto positionComponent = testComponent(objectClass, "position");
			testType(positionComponent, "Position");

			auto linearMoverComponent = testComponent(objectClass, "linearMover");
			testType(linearMoverComponent, "LinearMover");

			auto drawerComponent = testComponent(objectClass, "drawer");
			testType(drawerComponent, "SphereDrawer");
		}

		//uroll test
		
		auto worldScopename = unroll(comp.result.classes(), "Main", "main");

		//main instance
		auto unrolledMainInstace = testInstance(worldScopename, "main");

		//main.object instcance
		auto unrolledObjectExpr = unrolledMainInstace->scope().get("object");
		ENFORCE_MSG(unrolledObjectExpr, "No 'object' expression in main instance");
		const ObjectParser::InstanceHandle* unrolledObjectInstace = unrolledObjectExpr->cast<const ObjectParser::InstanceHandle>();
		ENFORCE_MSG(unrolledObjectInstace, "'object' expression in not ObjectParser::InstanceDefinitionExpression");
		const ComponentHandle* position = testComponent(unrolledObjectInstace, 0, "position", "Position");
		{
			auto positionExpr = position->scope().get("position");
			ENFORCE_MSG(positionExpr, "'No param 'position' in component");
			auto postionStruct = positionExpr->cast<EvalStruct>();
			ENFORCE_MSG(postionStruct, "'Param 'position' in no Struct type");

			Vector3 pos;
			Expressions::convertVar(*postionStruct, pos);
			ENFORCE(pos.x == -1);
		}

		const ComponentHandle* linearMover = testComponent(unrolledObjectInstace, 0, "linearMover", "LinearMover");
		{
			auto velExpr = linearMover->scope().get("velocity");
			ENFORCE_MSG(velExpr, "'No param 'velocity' in component");
			auto velStruct = velExpr->cast<EvalStruct>();
			ENFORCE_MSG(velStruct, "'Param 'velocity' in no Struct type");

			Vector3 vel;
			Expressions::convertVar(*velStruct, vel);
			ENFORCE(vel.x == 10);
		}

		const ComponentHandle* drawer = testComponent(unrolledObjectInstace, 0, "drawer", "SphereDrawer");
		{
			auto colorExpr = drawer->scope().get("color");
			ENFORCE_MSG(colorExpr, "'No param 'color' in component");
			auto colorStruct = colorExpr->cast<EvalStruct>();
			ENFORCE_MSG(colorStruct, "'Param 'color' in no Struct type");																														    

// 			ENFORCE_MSG(colorStruct->typeName() == "Color", "Param 'color' is no type 'Color'");
// 			const Expression* colorX = colorStruct->params[0];
// 
// 			const Expressions::Const<int>* velXFloat = colorX->cast<Const<int>>();
// 			ENFORCE(velXFloat->value == 1);
		}

		test(worldScopename, "main.three", Vector3(3, 3, 3));
		test(worldScopename, "main.object.acceleration", Vector3(3, 3, 3));		
		test(worldScopename, "main.object.derivativeFromVel", Vector3(20, 20, 20));
		test(worldScopename, "main.object.position2.position", Vector3(20, 20, 20));

		{
			const InstanceHandle* object = get(worldScopename, "main.object")->cast<InstanceHandle>();
			const EvaluationUnit* position = get(worldScopename, "main.object.position");
			const EvaluationUnit* posParam = get(worldScopename, "main.object.pos");

			bool isClassMemberPosition = object->scope().isClassMember(position);
			bool isClassMemberPosParam = object->scope().isClassMember(posParam);

			ENFORCE(isClassMemberPosition);
			ENFORCE(!isClassMemberPosParam);
		}



	}

}//

#endif