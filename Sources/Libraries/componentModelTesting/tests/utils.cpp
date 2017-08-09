// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "utils.h"
#include <boost/tokenizer.hpp>
#include <boost/token_iterator.hpp>
#include <iostream>

#include "componentLinkModelConfigurator.h"

namespace ComponentModelTesting
{
	using namespace Expressions;
	using namespace ObjectParser;

	const InstanceDefinitionExpression* findInstance(const ObjectParser::ClassDesc* cl, const std::string& instanceName)
	{
		const InstanceDefinitionExpression* result = cl->properties().at(instanceName)->cast<InstanceDefinitionExpression>();
		return result;

	}

	const Component* findComponent(const ObjectParser::ClassDesc* cl, const std::string& componentName)
	{
		const Component* result = cl->properties().at(componentName)->cast<Component>();
		return result;
	}

	const Component* testComponent(const ObjectParser::ClassDesc* cl, const std::string& componentName)
	{
		const Component* component = findComponent(cl, componentName);
		ENFORCE_MSG(component, "Can't find component '" + componentName + "' in '" + cl->typeName + "' class");

		return component;
	}

	bool testType(const ObjectParser::Component* cm, const std::string& type)
	{
		bool flag = cm->componentType == type;
		ENFORCE_MSG(flag, "Component '" + cm->name + "' is no type '" + type + "'")

		return flag;
	}

	const ComponentHandle* findComponent(const InstanceHandle* inst, int index, const std::string& componentName)
	{
		const Expression* expr = inst->get(componentName);
		ENFORCE_MSG(expr, "No '" + componentName + "' expression in '" + inst->name + "'");

		const ComponentHandle* compo = expr->cast<ComponentHandle>();
		ENFORCE_MSG(compo, "Epression '" + componentName + "' is no component");
		return compo;
	}


	const ComponentHandle* testComponent(const ObjectParser::InstanceHandle* inst, int index, const std::string& componentName, const std::string& componentType)
	{
		const ComponentHandle* compo = findComponent(inst, index, componentName);
		ENFORCE_MSG(compo->type == componentType, "Component '" + componentName + "' no type '" + componentType + "'");

		return compo;
	}


	bool testType(const ComponentHandle* cm, const std::string& type)
	{
		bool flag = cm->type == type;
		ENFORCE_MSG(flag, "Component '" + cm->name + "' is no type '" + type + "'")

		return flag;
	}

	bool testClassesCount(const ObjectParser::GrammarComposition& result, std::size_t count)
	{
		const ObjectParser::ClassTable& table = result.classes();
		ENFORCE_MSG(table.size() == count, str::stringize("incorrect classes count in ClassTable: ", table.size(), " expected: ", count).str());
		return table.size() == count;
	}

	Expressions::ScopeNames unroll(const ObjectParser::ClassTable& classTable, ObjectParser::ComponentModelConfigurator& configurator, const std::string& mainInstanceClassName, const std::string& mainInstanceName)
	{
		ObjectParser::ClassDesc* mainClass = classTable.get(mainInstanceClassName);
		ENFORCE(mainClass)

		Expressions::ScopeNames worldScopename;
		ObjectParser::InstanceDefinitionExpression mainInstace(mainInstanceClassName, mainInstanceName);
		ObjectParser::Unroller unroller(classTable, configurator, true);
		worldScopename.add("main", unroller.unrollInstance(&mainInstace, worldScopename), ScopeNames::INSERT);

		return worldScopename;
	}

	Expressions::ScopeNames unroll(const ObjectParser::ClassTable& classTable, const std::string& mainInstanceClassName, const std::string& mainInstanceName)
	{
		ComponentLinkModelConfigurator debugConfigurator;
		return unroll(classTable, debugConfigurator, mainInstanceClassName, mainInstanceName);
	}

	const ObjectParser::InstanceHandle* testInstance(Expressions::ScopeNames& scopeName, const std::string& instanceName)
	{
		const Expressions::Expression* unrolledMainExpr = scopeName.get("main");
		ENFORCE_MSG(unrolledMainExpr, "Main was not unroll");
		const ObjectParser::InstanceHandle* unrolledMainInstace = unrolledMainExpr->cast<const ObjectParser::InstanceHandle>();
		ENFORCE_MSG(unrolledMainInstace, "'main' expr in not ObjectParser::Instance");

		return unrolledMainInstace;
	}


	
	bool isIndex(const std::string& val, size_t& index)
	{
		char* end;
		index = strtoul(val.c_str(), &end, 10);
		return end != val.c_str() && (errno != ERANGE);
	}

	Expressions::Proxy* parse(const std::string& path)
	{
		Expressions::Proxy* proxy = Expressions::add<Expressions::Proxy>();
		
		typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
		boost::char_separator<char> sep(".[]");
		Tokenizer tokens(path, sep);
		for (auto& token : tokens)
		{
			size_t index;
			Proxy::PathElement* pathElement = 0;
			if (isIndex(token, index))
			{
				pathElement = new ArrayPath(index);
			}
			else
			{
				pathElement = new PropertyPath(token);

			}
			
			ENFORCE_MSG(pathElement, "");
			proxy->addPathElement(pathElement);

		}
		return proxy;
	}

	const Expressions::Expression* get(const Expressions::ScopeNames& scopename, const std::string& path)
	{
		Proxy* proxy = parse(path);
		return proxy->evaluated(scopename);
	}

}//



// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>
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