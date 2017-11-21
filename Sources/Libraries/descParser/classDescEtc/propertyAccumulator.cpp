#include "propertyAccumulator.h"

namespace ObjectParser
{

void PropertyAccumulator::bindRuntimeProperty(const std::string& paramName, Expressions::Reference* path, RuntimeDirection direction)
{

	runtimeProps.push_back(RuntimeProperty());
	RuntimeProperty& prop = runtimeProps[runtimeProps.size() - 1];

	prop.paramName = paramName;
	prop.path = path;
	prop.direction = direction;
}


void PropertyAccumulator::clear()
{
	runtimeProps.clear();
}

}//