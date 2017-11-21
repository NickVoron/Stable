#include "grammarComposition.h"
#include "classDesc.h"
#include "../compiler/compiler.h"
#include "expressions/library.include.h"
#include "expressions/componentRefConverter.h"
#include "expressions/componentPath.h"


namespace ObjectParser
{
using namespace Expressions;

static bool debugGC = false;

GrammarComposition::GrammarComposition(Compiler& compiler, ExtensionTable& extensions) :
	extensions(extensions),
	compiler(compiler)
{
	beginClass();
}


Expressions::Array* GrammarComposition::newArray(const std::unique_ptr<Expressions::ConstExprList>& params)
{
	if (debugGC) LOG_MSG(__FUNCTION__);

	return params ? Expressions::add<Expressions::Array>(*params) : Expressions::add<Expressions::Array>();
}

Expressions::Struct* GrammarComposition::newStruct(const std::string& type, const ConstExprList& params)
{
	if (debugGC) LOG_MSG(__FUNCTION__);

	return Expressions::add<Expressions::Struct>(type, params);
}

Expressions::Lambda* GrammarComposition::newLambda()
{
	return Expressions::add<Expressions::Lambda>();
}

Expressions::Lambda* GrammarComposition::newLambda(const Expressions::Expression* expr)
{
	return Expressions::add<Expressions::Lambda>(expr);
}

void GrammarComposition::fillLambdaParams(Expressions::Lambda* lambda, const Expressions::ConstExprList* params)
{
	ENFORCE_POINTER(lambda);
	ENFORCE_POINTER(params);
	lambda->assignParams(*params);
}

void GrammarComposition::fillLambdaCaptures(Expressions::Lambda* lambda, const Expressions::ConstExprList* params)
{
	ENFORCE_POINTER(lambda);
	ENFORCE_POINTER(params);
	lambda->assignCapture(*params);
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

//components
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
	
PropertyAssignment* GrammarComposition::newPropertyAssignment(const std::string& name, Expressions::Expression* value)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", name: " << name << ", value: " << value->string());

	PropertyAssignment* assigment = new PropertyAssignment(name, value);
	return assigment;
}

PropertyAssignment*	GrammarComposition::newPropertyAssignment(Expressions::Expression* value)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", name: anonymously , value: " << value->string());

	return new PropertyAssignment(ComponentNameSource::strGuid("anonymously"), value);
}

PropertyAssignmentList* GrammarComposition::newPropertyAssignmentsList(PropertyAssignment* assignment)
{
	PropertyAssignmentList* list = new PropertyAssignmentList();

	list->emplace_back(assignment);
	return list;
}

PropertyAssignmentList*  GrammarComposition::addPropertyAssignments(PropertyAssignmentList* propertyAssignList, PropertyAssignment* assignment)
{
	ENFORCE_POINTER(assignment);
	propertyAssignList->emplace_back(assignment);
	return propertyAssignList;
}

//addProperty
void GrammarComposition::addProperty(const std::string& name, Expressions::Expression* expr)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", name: " << name << ", expr: " << expr->string());
		
	currentClass().addProperty(name, expr);
}

void GrammarComposition::addProperty(const std::string& name)
{
	addProperty(name, Expressions::add<VoidExpression>());
}

void GrammarComposition::addProperty(Expressions::Expression* defaultExpr)
{
	addProperty(ComponentNameSource::strGuid("anonymously_property"), defaultExpr);
}

InstanceDefinitionExpression* GrammarComposition::newInstance(const std::string& typeName, const std::string& name, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList)
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
	return expr;
}

InstanceDefinitionExpression* GrammarComposition::newInstance(const std::string& typeName, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList)
{
	return newInstance(typeName, ComponentNameSource::strGuid("anonymously_instance"), propertyAssignmentList);
}

InstanceDefinitionExpression* GrammarComposition::newInstanceArray(const std::string& typeName, const std::string& name, Expressions::Expression* arrayParam, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", typeName: " << typeName << ", name: " << name << ", arrayParam: " << arrayParam->string());

	InstanceDefinitionExpression* expr = Expressions::add<InstanceDefinitionExpression>(typeName, name, propertyAssignmentList ? *propertyAssignmentList : PropertyAssignmentList(), false);

	expr->arrayData = arrayParam;
	currentClass().addInstance(expr);
	return expr;
}

InstanceDefinitionExpression* GrammarComposition::newInstanceArray(const std::string& typeName, Expressions::Expression* arrayExpr, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList)
{
	return newInstanceArray(typeName, ComponentNameSource::strGuid("anonymously_instance_array"), arrayExpr, propertyAssignmentList);
}

Expressions::ExternalExtension* GrammarComposition::addExternalExtension(const std::string& extensionName, const std::string& text)
{												
	return  Expressions::add<Expressions::ExternalExtension>(extensionName, text);
}

Expressions::Function* GrammarComposition::newFunction(const std::string& name, const Expressions::ConstExprList& params)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", name: " << name << ", paramsCount: " << params.size());

	return Expressions::add<Function>(name, params);
}

Expressions::Callable* GrammarComposition::newCallable(const std::string& name, const std::unique_ptr<Expressions::ConstExprList>& params)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", name: " << name << ", paramsCount: " << params->size());

	auto callable = Expressions::add<Callable>(name, *params);
	currentClass().addCallable(callable);
	return callable;
}


InstanceDefinitionExpression* GrammarComposition::newPrototype(const std::string& typeName, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", typeName: " << typeName);

	return Expressions::add<InstanceDefinitionExpression>(typeName, "", propertyAssignmentList ? *propertyAssignmentList : PropertyAssignmentList(), true);
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

//proxy
Expressions::Reference* GrammarComposition::newProxy(ObjectParser::location& lloc)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", this");
	return newProxy("this", lloc);
}

Expressions::Reference* GrammarComposition::newProxy(Expressions::Expression* expr, ObjectParser::location& lloc)
{
	return setloc(Expressions::add<Reference>(expr), lloc);
}

Expressions::Reference* GrammarComposition::newProxy(const std::string& targetName, ObjectParser::location& lloc)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", targetName: " << targetName);

	return setloc(Expressions::add<Reference>(new PropertyPathElement(targetName)), lloc);
}

Expressions::Reference::PathElement* GrammarComposition::newProxyArrayPath(const std::unique_ptr<Expressions::ConstExprList>& params)
{		
	return new ArrayPathElement(*params);
}

Reference::PathElement* GrammarComposition::newProxyPropertyPath(const std::string& name)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", name: " << name);

	return new PropertyPathElement(name);
}

Expressions::Reference::PathElement* GrammarComposition::newProxyComponentPath(const std::string& componentType)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", componentType: " << componentType);

	return new ComponentPathElement(componentType);
}

Expressions::Reference::PathElement* GrammarComposition::newProxyCallablePath(Expressions::Callable* callable)
{
	if (debugGC) LOG_MSG(__FUNCTION__ << ", callable: " << callable->string());

	return new CallablePathElement(*callable);

}

const ClassTable& GrammarComposition::classes() const
{
	return classTable;
}

ClassTable& GrammarComposition::classes()
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
	
	currentClass().resolveCallables();
	classTable[_currentClass->typeName] = std::move(_currentClass);
	beginClass();
}

ClassDesc& GrammarComposition::currentClass()
{ 
	ENFORCE(_currentClass); 
	return *_currentClass; 
}

}//