#include "component.h"		
#include "classDesc.h"
#include "../unroll/unroller.h"
#include "../unroll/componentHandle.h"



namespace ObjectParser
{

//ComponentParam
ComponentParam::ComponentParam(const std::string& type_, const std::string& value_):
	type(type_),
	value(value_)
{

}

std::string Component::string() const
{
	if (bindedComponent)
	{
		return str::stringize("component type = ", componentType, ", name = ", name, ", bindedHandle = ", bindedComponent);
	}
	
	return str::stringize("component type = ", componentType, ", name = ", name, " bindedHandle = nullptr");	
}

Expressions::EvaluationUnit* Component::evaluated(Expressions::EvaluatedScope& namescope) const
{
	ComponentHandle* handle = Expressions::add<ComponentHandle>(*this, namescope, runtimeProps, linkParams, propertyAssigments);
	handle->name = name;
	handle->scope().scopeName = name;
	handle->type = componentType;

	return handle;
}

std::string Component::typeName() const
{
	return componentType;
}

//Components
const Component* Components::findComponent(const std::string& name) const
{
	const_iterator it = find(name);
	return (it != end()) ? it->second : 0;
}

Component* Components::addComponent(Component* component)
{
	return (*this)[component->name] = component;
}

void Components::copy(const Components& proto)
{
	//копирование компонент
	for(const Components::value_type& compo : proto)
	{
		if(find(compo.first) != end()) //если в наследнике уже есть компонент с таким именем - значит он перекрывает родительский
			continue; 

		addComponent(compo.second);
	}
}

//
//Component* ComponentBuilder::create(const std::string& type, const std::string& name, const RuntimePropeties& runtimeProps)
//{
//	return create(type, name, runtimeProps, 0);
//}
//
Component* ComponentBuilder::create(const std::string& type, const std::string& name, ComponentParams& linkParams)
{
	const RuntimePropeties& runtimeProps = runtimePropetyAccum.runtimeProperties();

	Component* component = Expressions::add<Component>();
	component->componentType = type;
	component->name = name;

	//fill links like proxy
	for (auto& link: linkParams)
	{
		Expressions::Reference* linkProxy = Expressions::add<Expressions::Reference>();
		linkProxy->addPathElement(new Expressions::PropertyPathElement(link->value));
		component->linkParams.emplace_back(new PropertyAssignment(link->type, linkProxy));
	}
	component->runtimeProps = runtimeProps;

	//clear
	runtimeParams.clear();
	runtimePropetyAccum.clear();

	return component;
}

void ComponentBuilder::endRuntimeParams(ComponentParams& params_)
{
	runtimeParams = params_;
}



}//
