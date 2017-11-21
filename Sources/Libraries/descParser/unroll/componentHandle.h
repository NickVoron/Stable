#pragma once

#include "expressions/library.include.h"
#include "../classDescEtc/propertyAssignment.h"
#include "../classDescEtc/component.h"

namespace ObjectParser
{
	class ComponentHandle : public Expressions::EvaluationUnit
	{
	public:
		ComponentHandle(const Expressions::Expression& proto, Expressions::EvaluatedScope& parent):
			  Expressions::EvaluationUnit("component_handle", proto, parent)
			, links("links", nullptr)
		{}

		ComponentHandle(const Expressions::Expression& proto, Expressions::EvaluatedScope& parent, RuntimePropeties runtimeProps, PropertyAssignmentList links, PropertyAssignmentList params);

		virtual std::string typeName() const { return type; }
		virtual std::string string() const;

		virtual void extract(EvaluationSet& result) override;
		
		//дополнительное хранение связей
		RuntimePropeties& runtimeProperties() { return runtimePropsLarva;  }
		const RuntimePropeties& runtimeProperties() const { return runtimePropsLarva; }

		Expressions::EvaluatedScope links;
		std::string type;
		std::string name;
		std::optional<std::size_t> objectIndex;
		std::optional<std::size_t> componentIndex;

	private:
		// example
		// component[category = client] Logic logic(firstSpawner = unitSpawner, secondSpawner = buildingSpawner)
		// {
		//	param = a
		// }
		RuntimePropeties runtimePropsLarva;	// use for prefix "[category = client]"
		PropertyAssignmentList linksLarva;	// use for "(firstSpawner = unitSpawner, secondSpawner = buildingSpawner)"
		PropertyAssignmentList paramsLarva;	// use for "param = a"
	};

	class ValueAddressDesc : public Expressions::EvaluationUnit
	{
	public:
		ValueAddressDesc(const ComponentHandle& handle, std::size_t shift);

		const ComponentHandle& componentHandle;
		std::size_t componentShift;
	};


}//