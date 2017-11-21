#pragma once
#include "propertyAccumulator.h"
#include "propertyAssignment.h"
#include <boost/any.hpp>

namespace ObjectParser
{
	class ComponentHandle;

	// ComponentParam
	class ComponentParam
	{
	public:
		ComponentParam(const std::string& type_, const std::string& name_);
		
		std::string type;  
		std::string value;
	};

	struct ComponentParams : public std::vector<std::unique_ptr<ComponentParam>>
	{
		ComponentParams() {}
		ComponentParams(const ComponentParams& params) { operator=(params); }
		ComponentParams& operator=(const ComponentParams& params)
		{
			for (auto& p : params)
			{
				emplace_back(new ComponentParam(p->type, p->value));
			}

			return *this;
		}
	};

	class ClassDesc;
	//components
	class Component : public Expressions::Expression
	{
	public:
		virtual std::string string() const override;
		virtual Expressions::EvaluationUnit* evaluated(Expressions::EvaluatedScope& namescope) const override;
		virtual std::string typeName() const override;

		std::string componentType;
		std::string name;

		RuntimePropeties runtimeProps;
		//ComponentParams runtimeParams;
		PropertyAssignmentList linkParams;
		PropertyAssignmentList propertyAssigments;

		ComponentHandle* bindedComponent = nullptr; // заполнится указателем на созданный в компонентной модели компонентом при раскрутки 
	};

	struct Components : public std::unordered_map<std::string, Component*>
	{
		void copy(const Components& src);
		Component* addComponent(Component* component);							
		const Component* findComponent(const std::string& name) const;
	};
	
	//component builder
	class ComponentBuilder
	{
	public:
		Component* create(const std::string& type, const std::string& name, ComponentParams& linkParams);
	 	void endRuntimeParams(ComponentParams& params);

		PropertyAccumulator runtimePropetyAccum;
	private:
		ComponentParams runtimeParams;
		
	};





}//