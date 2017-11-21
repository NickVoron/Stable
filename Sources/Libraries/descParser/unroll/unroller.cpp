#include "unroller.h"
#include "instanceHandle.h"

#include "expressions/library.include.h"


namespace ObjectParser
{

Unroller::Unroller(ClassTable& classes_, EntitiesList& entities_):
	classes(classes_),
	entities(entities_)
{

}


Expressions::EvaluationUnit* Unroller::unrollInstance(const InstanceDefinitionExpression& instanceDefinition, Expressions::EvaluatedScope& namescope)
{
	InstanceHandle::unroller = this;
	Expressions::EvaluationUnit* unit = Expressions::unrollExpression(instanceDefinition, namescope, false);
	return unit->cast<InstanceHandle>();
}

std::unique_ptr<Expressions::EvaluatedScope> unroll(ClassTable& classTable, const InstanceDefinitionExpression& instance)
{
	EntitiesList entities;
	return std::move(unroll(classTable, entities, instance, nullptr));
}

std::unique_ptr<Expressions::EvaluatedScope> unroll(ClassTable& classTable, const std::string& mainInstanceClassName, const std::string& mainInstanceName)
{
	EntitiesList entities;
	return std::move(unroll(classTable, entities, mainInstanceClassName, mainInstanceName));
}

std::unique_ptr<Expressions::EvaluatedScope> unroll(ClassTable& classTable, EntitiesList& target, const InstanceDefinitionExpression& instance, Expressions::EvaluatedScope* parent)
{
	auto mainClass = classTable.get(instance.type);
	ENFORCE_POINTER(mainClass);

	Unroller unroller(classTable, target);
	std::unique_ptr<Expressions::EvaluatedScope> world(new Expressions::EvaluatedScope("world", parent));
	world->add("main", unroller.unrollInstance(instance, *world), Expressions::InsertMethod::INSERT);
	return std::move(world);
}

std::unique_ptr<Expressions::EvaluatedScope> unroll(ClassTable& classTable, EntitiesList& target, const std::string& mainInstanceClassName, const std::string& mainInstanceName)
{
	auto definition = Expressions::add<InstanceDefinitionExpression>(mainInstanceClassName, mainInstanceName);
	return std::move(unroll(classTable, target, *definition, nullptr));
}

}//