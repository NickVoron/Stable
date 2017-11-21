#pragma once
#include "classDesc.h"
#include "expressions/library.include.h"
#include "propertyAccumulator.h"
#include "propertyAssignment.h"
#include <memory>
#include "../parser/location.hh"

namespace ObjectParser
{
	class ExtensionTable;
	class Compiler;

	template<class ExpressionType>
	inline ExpressionType* setloc(ExpressionType* expr, const ObjectParser::location& lloc)
	{
		expr->loc.line = lloc.begin.line;
		expr->loc.filename = *lloc.begin.filename;
		return expr;
	}

	class GrammarComposition
	{
		friend class Constructor;
		friend class Compiler;
		friend class Parser;
		friend class Lexer;

	public:
		GrammarComposition(Compiler& compiler, ExtensionTable& extensions);

		//expressions
		//const
		template<class T>
		Expressions::ExpressionConst<T>* newConst(const T& value, ObjectParser::location& lloc)
		{
			//LOG_MSG("begin line: " << lloc.begin.line << ", col:" << lloc.begin.column << ", end line:" << lloc.end.line << ", col:" << lloc.end.column);
			return Expressions::add<Expressions::ExpressionConst<T>>(value);
		}
	
		//expr
		Expressions::Function* newFunction(const std::string& name, const Expressions::ConstExprList& params);
		Expressions::Callable* newCallable(const std::string& name, const std::unique_ptr<Expressions::ConstExprList>& params);

		//Expressions::TernaryOperator* newTernaryOperator(Expressions::Expression* op0, Expressions::Expression* op1, Expressions::Expression* op2);
		Expressions::Array* newArray(const std::unique_ptr<Expressions::ConstExprList>& params);
		Expressions::Struct* newStruct(const std::string& type, const Expressions::ConstExprList& params);

		Expressions::Lambda* newLambda();
		Expressions::Lambda* newLambda(const Expressions::Expression* expr);
		void fillLambdaParams(Expressions::Lambda* lambda, const Expressions::ConstExprList* params);
		void fillLambdaCaptures(Expressions::Lambda* lambda, const Expressions::ConstExprList* params);
		
		Expressions::PropertiesStruct* newPropertiesStruct(const std::string& name, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignment);
		Expressions::ExternalExtension* addExternalExtension(const std::string& extensionName, const std::string& text);

		//prototype
		InstanceDefinitionExpression* newPrototype(const std::string& typeName, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList);

		//instance
		InstanceDefinitionExpression* newInstance(const std::string& typeName, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList);
		InstanceDefinitionExpression* newInstance(const std::string& typeName, const std::string& name, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList);
		InstanceDefinitionExpression* newInstanceArray(const std::string& typeName, Expressions::Expression* arrayExpr, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList);
		InstanceDefinitionExpression* newInstanceArray(const std::string& typeName, const std::string& name, Expressions::Expression* arrayExpr, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList);

		//component
		Component* newComponent(const std::string& type, const std::string& name);
		Component* newComponent(const std::string& type, const std::string& name, const std::unique_ptr<ComponentParams>&  params);

		Component* addComponent(Component* component, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList);

		void endComponentRuntimeParams(const std::unique_ptr<ComponentParams>& params);

		//proxy
		Expressions::Reference* newProxy(ObjectParser::location& lloc);
		Expressions::Reference* newProxy(Expressions::Expression* expr, ObjectParser::location& lloc);
		Expressions::Reference* newProxy(const std::string& targetName, ObjectParser::location& lloc);

		Expressions::Reference::PathElement* newProxyArrayPath(const std::unique_ptr<Expressions::ConstExprList>& params);
		Expressions::Reference::PathElement* newProxyPropertyPath(const std::string& name);
		Expressions::Reference::PathElement* newProxyComponentPath(const std::string& name);
		Expressions::Reference::PathElement* newProxyCallablePath(Expressions::Callable* callable);

		//class property
		void addProperty(const std::string& name);
		void addProperty(const std::string& name, Expressions::Expression* defaultExpr);
		void addProperty(Expressions::Expression* defaultExpr);

		//property assignment описывалект какие проперти какими значениями заполняются, внутри инстанса 
		PropertyAssignmentList*		newPropertyAssignmentsList(PropertyAssignment* assignment);
		PropertyAssignment*			newPropertyAssignment(Expressions::Expression* value);
		PropertyAssignment*			newPropertyAssignment(const std::string& name, Expressions::Expression* value);
		PropertyAssignmentList*		addPropertyAssignments(PropertyAssignmentList* propertyAssignList, PropertyAssignment* assignment);

		//
		void setClassName(const std::string& type, bool iface);

		//inheritance
		//mix
		void addMixInheriatance(const std::string& type, const std::unique_ptr<PropertyAssignmentList>& propertyAssignmentList);

		const ClassTable& classes() const;
		ClassTable& classes();

		ExtensionTable& extensions;
		Compiler& compiler;

		void beginClass();
		void endClass();
	private:
		ClassDesc& currentClass();

		ClassTable classTable;
		std::unique_ptr<ClassDesc> _currentClass;

		ComponentBuilder compoBuilder;
	};

}//