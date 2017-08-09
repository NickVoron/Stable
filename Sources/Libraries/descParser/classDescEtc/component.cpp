// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "component.h"		
#include "classDesc.h"
#include "unroller.h"



namespace ObjectParser
{


ComponentParam::ComponentParam(const std::string& type_, const std::string& value_):
	type(type_),
	value(value_)
{

}


Component::~Component()
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

const Expressions::Expression* Component::evaluated(const Expressions::ScopeNames& parentScopenames, boost::any* userData) const
{
	Unroller* unroller = boost::any_cast<Unroller*>(*userData);
	ENFORCE_MSG(unroller, "");
	return unroller->unrollComponent(const_cast<Expressions::ScopeNames&>(parentScopenames), *this);
}

std::string Component::typeName() const
{
	return componentType;
}

Expressions::References Component::references() const
{
	Expressions::References resultRefs;

	const Expressions::References& propertiesRefs = propertyAssigments.references();
	resultRefs.insert(resultRefs.begin(), propertiesRefs.begin(), propertiesRefs.end());
	
	const Expressions::References& linksRefs = linkParams.references();
	resultRefs.insert(resultRefs.begin(), linksRefs.begin(), linksRefs.end());

	return resultRefs;
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
	component->runtimeParams = runtimeParams;

	
	for (auto& link: linkParams)
	{
		Expressions::Proxy* linkProxy = Expressions::add<Expressions::Proxy>();
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