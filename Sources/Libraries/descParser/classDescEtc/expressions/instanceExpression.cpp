#include "instanceExpression.h"
#include "../../unroll/unroller.h"
#include "../../unroll/prototypeHandle.h"
#include "../../unroll/instanceHandle.h"
#include "../classDesc.h"

namespace ObjectParser
{

using namespace Expressions;

//
InstanceDefinitionExpression::InstanceDefinitionExpression(const std::string& type_, const std::string& name_, const PropertyAssignmentList& params_, bool noinst):
	type(type_),
	name(name_),
	params(params_),
	noinstance(noinst)
	{
	}

InstanceDefinitionExpression::InstanceDefinitionExpression(const std::string& type_, const std::string& name_):
	InstanceDefinitionExpression(type_, name_, PropertyAssignmentList(), false)
{
}

std::string InstanceDefinitionExpression::string() const
{
	auto value = str::stringize("InstanceDefinitionExpression type: ", type, ", name: ", name).str();

	if (arrayData)
	{
		value += str::stringize(", arrayData:", arrayData->string()).str();
	}

	return value;
}

InstanceDefinitionExpression* InstanceDefinitionExpression::instance() const
{
	return add<InstanceDefinitionExpression>(type, name, params, false);
}

// в инстансе уже храняться правильные значения его входящих параметров, они уже вычислинны их нужно только взять и 
// забиндить в класс инстанса
EvaluationUnit* InstanceDefinitionExpression::evaluated(EvaluatedScope& namescope) const
{
	Unroller* unroller = InstanceHandle::unroller;
	ENFORCE_POINTER(unroller);

	EvaluationUnit* result = nullptr;

	const ClassDesc* instanceClass = unroller->classes.get(type);
	ENFORCE_MSG(instanceClass, str::spaced("class:", type, "not exist"));

	if (!noinstance)
	{
		result = add<InstanceHandle>(*instanceClass, *this, namescope);
	}
	else
	{
		result = add<PrototypeHandle>(namescope, *this, unroller->classes, unroller->entities);
	}

	return result;
}


}