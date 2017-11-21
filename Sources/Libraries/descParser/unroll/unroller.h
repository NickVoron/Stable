#pragma once

#include <string>

#include "../classDescEtc/grammarComposition.h"
#include "../classDescEtc/expressions/instanceExpression.h"


namespace ObjectParser
{
	class Unroller
	{
	public:
		Unroller(ClassTable& classes, EntitiesList& target);
		Expressions::EvaluationUnit* unrollInstance(const InstanceDefinitionExpression& prototype, Expressions::EvaluatedScope& namescope);

		ClassTable& classes;
		EntitiesList& entities;
	};

	std::unique_ptr<Expressions::EvaluatedScope> unroll(ClassTable& classTable, const InstanceDefinitionExpression& instance);
	std::unique_ptr<Expressions::EvaluatedScope> unroll(ClassTable& classTable, const std::string& mainInstanceClassName, const std::string& mainInstanceName);
	std::unique_ptr<Expressions::EvaluatedScope> unroll(ClassTable& classTable, EntitiesList& target, const InstanceDefinitionExpression& instance, Expressions::EvaluatedScope* parent);
	std::unique_ptr<Expressions::EvaluatedScope> unroll(ClassTable& classTable, EntitiesList& target, const std::string& mainInstanceClassName, const std::string& mainInstanceName);
}