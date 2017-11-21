#pragma once

#include "defaultLogs/library.include.h"
#include "stuff/library.include.h"
#include "containers/library.include.h"
#include "common/apply.h"


struct ExecutionUnitBase : public Base::IntrusiveList<ExecutionUnitBase>::Node 
{ 
	virtual ~ExecutionUnitBase(){}

	virtual void dump() = 0;
	virtual void execute() = 0;
	virtual void release() = 0;
	virtual void add(ComponentBase* component) = 0;
};
	
template<class Component, class ResultList>
class ComponentsList : public Base::IntrusiveList<Component, Component>
{
public:
	typedef Component ComponentType;

	struct ExecutionUnit : public ExecutionUnitBase
	{
		virtual void dump()  override { LOG_INFO(Component::ClassName()); };
		virtual void release() override	{ }
		virtual void add(ComponentBase* component)  override { ((Component*) component)->parent_list = &components; }

		ResultList components;
	};
};


template<class Component, class ParametersType = void>
class UpdatableComponentsList : public ComponentsList<Component, UpdatableComponentsList<Component, ParametersType> >
{
public:
	void execute(ParametersType& parameters)
	{
		for (auto c = this->begin(); c != this->end(); )
		{
			auto next = c;
			++next;
			stl::apply([c](auto&... p)
			{
				c->update(p...);
			}, parameters);
			c = next;
		}
	}

	struct ExecutionUnit : public ComponentsList<Component, UpdatableComponentsList<Component, ParametersType>>::ExecutionUnit  
	{ 
		ExecutionUnit(ParametersType& params) : parameters(params) {}
		virtual void execute() 
		{ 
			this->components.execute(parameters);
		}

		ParametersType& parameters;
	};	
};

template<class Component>
class UpdatableComponentsList<Component, void> : public ComponentsList<Component, UpdatableComponentsList<Component, void> >
{
public:									
	void execute()
	{
		for (auto c = this->begin(); c != this->end(); )
		{
			auto next = c;
			++next;
			c->update();
			c = next;
		}
	}

	struct ExecutionUnit : public ComponentsList<Component, UpdatableComponentsList<Component, void>>::ExecutionUnit 
	{ 
		virtual void execute() 
		{
			this->components.execute();
		}
	};
};


template<class ComponentType, class ReturnType, class... P> std::tuple<std::decay_t<P>...> tuple_deduction_f(ReturnType(ComponentType::*proc)(P...));

template<class FunctionType, FunctionType func>
auto& function_parameters()
{
	static decltype(tuple_deduction_f(func)) params;
	return params;
}


template<class ComponentType>
auto& parameters()
{
	return function_parameters<decltype(&ComponentType::update), &ComponentType::update>();
}

template<class Component>
class UpdatableComponentInputParameters
{
public:
	static ExecutionUnitBase* executionUnit() { return new typename UpdatableComponentsList<Component, decltype(parameters<Component>())>::ExecutionUnit( parameters<Component>() ); }
};

template<class Component, class ParametersType = void>
class SimpleComponentInputParameters : public Base::MeyersSingleton<SimpleComponentInputParameters<Component, ParametersType>>
{
public:
	ParametersType parameters;
};

template<class Component>
class SimpleComponentInputParameters<Component, void> : public Base::MeyersSingleton<SimpleComponentInputParameters<Component, void>>
{
};
