// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "componentLinkModelConfigurator.h"

#include <map>
#include <vector>
#include <memory>

namespace ComponentModelTesting
{
	using namespace Expressions;
	using namespace ObjectParser;

	ComponentLinkModelConfigurator::ComponentLinkModelConfigurator():
		world("world"),
		currInst(&world)
	{

	}


	void ComponentLinkModelConfigurator::beginCreateInstance(const std::string& name, const std::string& type)
	{
		
		logstack++;

		DebugInstanceHandle* handle = Expressions::add<DebugInstanceHandle>(name);
		currInst->type = type;
		handle->parent = currInst;

		if (currInst->exists(name))
		{
			
			const Expressions::Expression* firstInstance = currInst->get(name);
			const DebugArray* constArray = dynamic_cast<const DebugArray*>(firstInstance);
			if (constArray) 
			{
				DebugArray* array = const_cast<DebugArray*>(constArray);
				array->add(handle);
			}
			else
			{
			
				currInst->erase(name);
				DebugArray* arrayInstances = add<DebugArray>();
				arrayInstances->add(firstInstance);
				arrayInstances->add(handle);

				currInst->add(name, arrayInstances, Expressions::ScopeNames::INSERT);
			}

			DebugInstanceHandle* handle = Expressions::add<DebugInstanceHandle>(name);

		}
		else
		{
			currInst->add(name, handle, Expressions::ScopeNames::INSERT);
		}

		currInst = handle;
	}

	InstanceHandle* ComponentLinkModelConfigurator::endCreateInstance()
	{
		ENFORCE(currInst);
		DebugInstanceHandle* handle = currInst;
		logstack--;
		
		
		currInst = currInst->parent;
		
		return handle;
	}

	ObjectParser::ComponentHandle* ComponentLinkModelConfigurator::preCreateComponent(const std::string& type, const std::string& name)
	{
		ENFORCE(currInst);

		ComponentHandle* comp = Expressions::add<ComponentHandle>();
		currInst->add(name, comp, Expressions::ScopeNames::INSERT);

		currComp = comp;

		currComp->name = name;
		currComp->type = type;

		++logstack;
		
		compStack.push_back(currComp);

		return currComp;

	}

	void ComponentLinkModelConfigurator::beginCreateComponent(ObjectParser::ComponentHandle* handle, const std::string& type, const std::string& name)
	{
		ENFORCE(currComp && currComp->name == name);
		ENFORCE(currComp && currComp->type == type);
		
	}

	void ComponentLinkModelConfigurator::bindRuntimeProperty(ObjectParser::ComponentHandle* handle, const std::string& name, const std::string& value)
	{

	}

	void ComponentLinkModelConfigurator::bindLink(ObjectParser::ComponentHandle* handle, const std::string& name, const ComponentHandle* value)
	{
		ENFORCE(currComp);
		currComp->links.emplace(name, value);
	}

	void ComponentLinkModelConfigurator::bindComponentProperty(ObjectParser::ComponentHandle* handle, const std::string& name, const Expressions::Expression* value)
	{
		ENFORCE(currComp);
		currComp->add(name, value, Expressions::ScopeNames::INSERT);
		
	}

	void ComponentLinkModelConfigurator::endCreateComponent(ObjectParser::ComponentHandle* handle)
	{
		ENFORCE(currComp);

		ENFORCE(handle == currComp);
		
		--logstack;

		compStack.pop_back();
		if (!compStack.empty())
		{
			currComp = compStack.back();
		}
		else
		{
			currComp = 0;
		}
	}

}//



// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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