// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "component.h"		
#include "classDesc.h"
#include "../unroll/unroller.h"
#include "../unroll/componentHandle.h"



namespace ObjectParser
{


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

Expressions::EvaluationUnit* Component::evaluated(const Expressions::EvaluatedScope& parentScopenames, boost::any* userData) const
{
	ComponentHandle* handle = Expressions::add<ComponentHandle>(runtimeProps, linkParams, propertyAssigments);
	handle->name = name;
	handle->scopeName = name;
	handle->type = componentType;

	return handle;
}

std::string Component::typeName() const
{
	return componentType;
}


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
	
	for(const Components::value_type& compo : proto)
	{
		if(find(compo.first) != end()) 
			continue; 

		addComponent(compo.second);
	}
}







Component* ComponentBuilder::create(const std::string& type, const std::string& name, ComponentParams& linkParams)
{
	const RuntimePropeties& runtimeProps = runtimePropetyAccum.runtimeProperties();

	Component* component = Expressions::add<Component>();
	component->componentType = type;
	component->name = name;

	
	for (auto& link: linkParams)
	{
		Expressions::Reference* linkProxy = Expressions::add<Expressions::Reference>();
		linkProxy->addPathElement(new Expressions::PropertyPath(link->value));
		component->linkParams.emplace_back(new PropertyAssignment(link->type, linkProxy));
	}
	component->runtimeProps = runtimeProps;

	
	runtimeParams.clear();
	runtimePropetyAccum.clear();

	return component;
}

void ComponentBuilder::endRuntimeParams(ComponentParams& params_)
{
	runtimeParams = params_;
}



}




// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.