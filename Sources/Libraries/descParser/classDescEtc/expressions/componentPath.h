#pragma once
#include "expressions/library.include.h"


namespace ObjectParser
{
	class InstanceHandle;

	class ComponentPathElement: public Expressions::Reference::PathElement
	{
	public:
		ComponentPathElement(const std::string& componentType_) : componentType(componentType_) {}
		virtual std::string string() const override { return str::stringize("<", componentType, ">"); }
		virtual Expressions::EvaluationUnit* evaluated(Expressions::EvaluatedScope& namescope) const;
		virtual Expressions::BaseIndexerUnit* createIndexer(Expressions::EvaluatedScope& namescope) const;
		virtual Expressions::BaseSamplerUnit* createSampler(Expressions::EvaluatedScope& namescope) const;
		
		const std::string componentType;
	};

	//
	struct ComponentPathElementUnit : public Expressions::EvaluationUnit
	{
		ComponentPathElementUnit(Expressions::EvaluatedScope& parent, const ComponentPathElement& element_) : Expressions::EvaluationUnit("component_path_element_unit", element_, parent), element(element_) {}

		virtual std::string string() const override { return element.string(); }

		const ComponentPathElement& element;
	};

	//
	class ComponentTypeSamplerUnit : public Expressions::BaseSamplerUnit
	{
	public:
		ComponentTypeSamplerUnit(Expressions::EvaluatedScope& parent, const ComponentPathElement& proto);

		virtual Expressions::EvaluationUnit* sample(Expressions::EvaluationUnit* source) const override;
		const ComponentPathElement& element;
	};
}
