#pragma once
#include "expressions/library.include.h"
#include "componentModel/library.include.h"

#include "prototypeHandle.h"
#include "../classDescEtc/expressions/instanceExpression.h"


namespace ObjectParser
{
	class PrototypeHandle : public Expressions::EvaluationUnit
	{
	public:
		PrototypeHandle(Expressions::EvaluatedScope& namescope, const InstanceDefinitionExpression& prototype, ClassTable& classTable, EntitiesList& entities);
		
		virtual Expressions::EvaluationInfo evaluate() override;
		virtual void extract(EvaluationSet& result) override;
		
		const InstanceDefinitionExpression& prototype;
		ClassTable& classTable;
		EntitiesList& entities;
		EvaluationUnit* object = nullptr;
	};	 
}