#include "componentHandle.h"
#include "prototypeHandle.h"
#include "evaluation.h"

namespace ObjectParser
{
using namespace Expressions;

ComponentHandle::ComponentHandle(const Expression& proto, Expressions::EvaluatedScope& parent, RuntimePropeties runtimeProps_, PropertyAssignmentList links_, PropertyAssignmentList params_):
	ComponentHandle(proto, parent)
{
	runtimePropsLarva = runtimeProps_;
	linksLarva = links_;
	paramsLarva = params_;

	Expressions::prepareEvaluationUnits(parent, linksLarva, links, false);
	Expressions::prepareEvaluationUnits(parent, paramsLarva, scope(), false);
}

void ComponentHandle::extract(EvaluationSet& result)
{
	EvaluationUnit::extract(result);
	Expressions::extractDependencies(links, result);
}

std::string ComponentHandle::string() const
{
	return str::spaced("component:", type, "name:", name).str();
}

//
//
//
ValueAddressDesc::ValueAddressDesc(const ComponentHandle& handle, std::size_t shift) :
	EvaluationUnit("value_address_desc", handle.protoexpr, EvaluationUnit::commonParent)
	, componentHandle(handle)
	, componentShift(shift) 
{
}

}