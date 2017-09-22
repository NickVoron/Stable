// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "grammarComposition.h"
#include "classDesc.h"
#include "../compiler/compiler.h"
#include "expressions/library.include.h"
#include "expressions/componentRefConverter.h"
#include "expressions/noValueExpression.h"
#include "expressions/componentPath.h"


namespace ObjectParser
{
using namespace Expressions;

static std::string globalScopeName("global");
  
GrammarComposition::GrammarComposition(Compiler& compiler, ExtensionTable& extensions) :
	extensions(extensions),
	compiler(compiler)
{
	beginClass();
}

GrammarComposition::~GrammarComposition()
{
}

Expressions::Function* GrammarComposition::newFunction(const std::string& name, const ConstExprList& params)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", name: " << name);
	return Expressions::add<Expressions::Function>(name, params);
}			   

Expressions::Array* GrammarComposition::newArray(const std::unique_ptr<Expressions::ConstExprList>& params)
{
	if (debugGC) LOG_MSG(__FUNCTION__);

	return Expressions::add<Expressions::Array>(*params);
}

Expressions::Struct* GrammarComposition::newStruct(const std::string& type, const ConstExprList& params)
{
	if (debugGC) LOG_MSG(__FUNCTION__);

	return Expressions::add<Expressions::Struct>(type, params);
}








PropertiesStruct* GrammarComposition::newPropertiesStruct(const std::string& name, const std::unique_ptr<PropertyAssignmentList>& propertyAssignment)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", name: " << name);

	PropertiesStruct* res = Expressions::add<PropertiesStruct>(name);
	
	if (propertyAssignment)
	{
		for (auto& iter : *propertyAssignment)
		{
			res->add(iter->propertyName, iter->value);
		}
	}

	return res;
}


Component* GrammarComposition::newComponent(const std::string& type, const std::string& name)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", type: " << type << ", name: " << name);

	return newComponent(type, name, std::unique_ptr<ComponentParams>( new ComponentParams() ) );
}

Component* GrammarComposition::newComponent(const std::string& type, const std::string& name, const std::unique_ptr<ComponentParams>& params)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", type: " << type << ", name: " << name << ", paramsCount: " << params->size());

	return compoBuilder.create(type, name, *params);
}

Component* GrammarComposition::addComponent(Component* component, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList)
{
	if (debugGC) LOG_MSG(__FUNCTION__ );
	PropertyAssignmentList properties = propertyAssignmentList ? *propertyAssignmentList : PropertyAssignmentList();
	return currentClass().addComponent(component, properties);
}

void GrammarComposition::endComponentRuntimeParams(const std::unique_ptr<ComponentParams>& params)
{
	if (debugGC) LOG_MSG(__FUNCTION__);

	compoBuilder.endRuntimeParams(*params);
}


void GrammarComposition::addvar(const std::string& name, Expressions::Expression* expr)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", name: " << name << ", expr: " << expr->string());

	INCOMPLETE;
	
}

	
PropertyAssignment* GrammarComposition::newPropertyAssignment(const std::string& name, Expressions::Expression* value)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", name: " << name << ", value: " << value->string());

	PropertyAssignment* assigment = new PropertyAssignment(name, value);
	return assigment;
}

void GrammarComposition::addProperty(const std::string& name, Expressions::Expression* expr, bool isPrivate)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", name: " << name << ", expr: " << expr->string());
		
	currentClass().addProperty(name, expr);
}

void GrammarComposition::addProperty(const std::string& name)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", name: " << name);

	addProperty(name, Expressions::add<NoValueExpression>());
}

PropertyAssignmentList* GrammarComposition::newPropertyAssignmentsList(PropertyAssignment* assignment)
{
	PropertyAssignmentList* list =  new PropertyAssignmentList();

	list->emplace_back(assignment);
	return list;
}

PropertyAssignmentList*  GrammarComposition::addPropertyAssignments(PropertyAssignmentList* propertyAssignList, PropertyAssignment* assignment)
{
	propertyAssignList->emplace_back(assignment);
	return propertyAssignList;
}


void GrammarComposition::bindRuntimeProperty(const std::string& paramName, Expressions::Reference* path, RuntimeDirection direction)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", paramName: " << paramName << ", path: " << path->string() << ", direction: " << direction );

	compoBuilder.runtimePropetyAccum.bindRuntimeProperty(paramName, path, direction);
}

void GrammarComposition::newInstance(const std::string& typeName, const std::string& name, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", typeName: " << typeName <<  ", name: "<< name);

	InstanceDefinitionExpression* expr = nullptr;
		
	if (propertyAssignmentList)
	{
		expr = Expressions::add<InstanceDefinitionExpression>(typeName, name, *propertyAssignmentList, false);
	}
	else
	{
		expr = Expressions::add<InstanceDefinitionExpression>(typeName, name);
	}
			
	currentClass().addInstance(expr);
}

void GrammarComposition::newInstanceArray(const std::string& typeName, const std::string& name, Expressions::Expression* arrayParam, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", typeName: " << typeName << ", name: " << name << ", arrayParam: " << arrayParam->string());

	InstanceDefinitionExpression* expr = Expressions::add<InstanceDefinitionExpression>(typeName, name, propertyAssignmentList ? *propertyAssignmentList : PropertyAssignmentList(), false);

	expr->arrayData = arrayParam;
	currentClass().addInstance(expr);
}

Expressions::ExternalExtension* GrammarComposition::addExternalExtension(const std::string& extensionName, const std::string& text)
{												
	
	

	
	return 0;
}


Expressions::Expression* GrammarComposition::newFunctionOrStruct(const std::string& name, const std::unique_ptr<Expressions::ConstExprList>& params)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", name: " << name << ", paramsCount: " << params->size());

	Expression* expr = FUNCTIONS::exist(name) ? (Expression*)Expressions::add<Function>(name, *params) : (Expression*)Expressions::add<Struct>(name, *params);
	ENFORCE_MSG(expr, "No one function or struct finded");
		
	return expr;
}


InstanceDefinitionExpression* GrammarComposition::newPrototype(const std::string& typeName, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", typeName: " << typeName);

	InstanceDefinitionExpression* instance = Expressions::add<InstanceDefinitionExpression>(typeName, "", propertyAssignmentList ? *propertyAssignmentList : PropertyAssignmentList(), true);

	return instance;
}

void GrammarComposition::endCollection(const std::string& collectionName, const std::string& typeName)
{
	

	
	
	
	

	
}

void GrammarComposition::addCollectionElement(const std::string& name, ConstExprList* guidList)
{
	
	
}

void GrammarComposition::setClassName(const std::string& type, bool iface)
{
	currentClass().typeName = type;
	currentClass().isInterface = iface;
}

void GrammarComposition::addMixInheriatance(const std::string& type, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList)
{
	currentClass().addMixInheritance(type, propertyAssignmentList ? *propertyAssignmentList : PropertyAssignmentList());
}

void GrammarComposition::addAggregateInheritance(const std::string& type)
{
	currentClass().addAggregateInheritance(type);
}

void GrammarComposition::setInheritanceAssigment(const std::string& type, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList)
{
	currentClass().setParentAssigment(type, propertyAssignmentList ? *propertyAssignmentList : PropertyAssignmentList());
}


Expressions::Reference* GrammarComposition::newProxy()
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", this");
	return newProxy("this");
}

Expressions::Reference* GrammarComposition::newProxy(Expressions::Expression* expr)
{
	return Expressions::add<Reference>(expr);
}

Expressions::Reference* GrammarComposition::newProxy(const std::string& targetName)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", targetName: " << targetName);

	return Expressions::add<Reference>(new PropertyPath(targetName));
}

Expressions::Reference::PathElement* GrammarComposition::newProxyArrayPath(const std::unique_ptr<Expressions::ConstExprList>& params)
{		
	return new ArrayPath(*params);
}

Reference::PathElement* GrammarComposition::newProxyPropertyPath(const std::string& name)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", name: " << name);

	return new PropertyPath(name);
}

Expressions::Reference::PathElement* GrammarComposition::newProxyComponentPath(const std::string& componentType)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", componentType: " << componentType);

	return new ComponentPath(componentType);
}

const ClassTable& GrammarComposition::classes() const
{
	return classTable;
}

void GrammarComposition::beginClass()
{
	_currentClass = std::make_unique<ClassDesc>();
}

void GrammarComposition::endClass()
{
	ENFORCE(_currentClass);
	if (debugGC) LOG_MSG(__FUNCTION__ << ", typeName: " << _currentClass->typeName);
	
	classTable[_currentClass->typeName] = std::move(_currentClass);
	beginClass();
}

ClassDesc& GrammarComposition::currentClass()
{ 
	ENFORCE(_currentClass); 
	return *_currentClass; 
}












}//



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