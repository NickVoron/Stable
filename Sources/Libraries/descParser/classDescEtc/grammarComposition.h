// Copyright (C) 2016-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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

		
		
		template<class T>
		Expressions::ExpressionConst<T>* newConst(const T& value, ObjectParser::location& lloc)
		{
			
			return Expressions::add<Expressions::ExpressionConst<T>>(value);
		}
	
		
		Expressions::Function* newFunction(const std::string& name, const Expressions::ConstExprList& params);
		Expressions::Callable* newCallable(const std::string& name, const std::unique_ptr<Expressions::ConstExprList>& params);

		
		Expressions::Array* newArray(const std::unique_ptr<Expressions::ConstExprList>& params);
		Expressions::Struct* newStruct(const std::string& type, const Expressions::ConstExprList& params);

		Expressions::Lambda* newLambda();
		Expressions::Lambda* newLambda(const Expressions::Expression* expr);
		void fillLambdaParams(Expressions::Lambda* lambda, const Expressions::ConstExprList* params);
		void fillLambdaCaptures(Expressions::Lambda* lambda, const Expressions::ConstExprList* params);
		
		Expressions::PropertiesStruct* newPropertiesStruct(const std::string& name, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignment);
		Expressions::ExternalExtension* addExternalExtension(const std::string& extensionName, const std::string& text);

		
		InstanceDefinitionExpression* newPrototype(const std::string& typeName, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList);

		
		InstanceDefinitionExpression* newInstance(const std::string& typeName, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList);
		InstanceDefinitionExpression* newInstance(const std::string& typeName, const std::string& name, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList);
		InstanceDefinitionExpression* newInstanceArray(const std::string& typeName, Expressions::Expression* arrayExpr, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList);
		InstanceDefinitionExpression* newInstanceArray(const std::string& typeName, const std::string& name, Expressions::Expression* arrayExpr, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList);

		
		Component* newComponent(const std::string& type, const std::string& name);
		Component* newComponent(const std::string& type, const std::string& name, const std::unique_ptr<ComponentParams>&  params);

		Component* addComponent(Component* component, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList);

		void endComponentRuntimeParams(const std::unique_ptr<ComponentParams>& params);

		
		Expressions::Reference* newProxy(ObjectParser::location& lloc);
		Expressions::Reference* newProxy(Expressions::Expression* expr, ObjectParser::location& lloc);
		Expressions::Reference* newProxy(const std::string& targetName, ObjectParser::location& lloc);

		Expressions::Reference::PathElement* newProxyArrayPath(const std::unique_ptr<Expressions::ConstExprList>& params);
		Expressions::Reference::PathElement* newProxyPropertyPath(const std::string& name);
		Expressions::Reference::PathElement* newProxyComponentPath(const std::string& name);
		Expressions::Reference::PathElement* newProxyCallablePath(Expressions::Callable* callable);

		
		void addProperty(const std::string& name);
		void addProperty(const std::string& name, Expressions::Expression* defaultExpr);
		void addProperty(Expressions::Expression* defaultExpr);

		
		PropertyAssignmentList*		newPropertyAssignmentsList(PropertyAssignment* assignment);
		PropertyAssignment*			newPropertyAssignment(Expressions::Expression* value);
		PropertyAssignment*			newPropertyAssignment(const std::string& name, Expressions::Expression* value);
		PropertyAssignment*			newConnection(Expressions::Reference* ref, Expressions::Expression* value, Connection::Direction dir);
		PropertyAssignmentList*		addPropertyAssignments(PropertyAssignmentList* propertyAssignList, PropertyAssignment* assignment);

		
		void setClassName(const std::string& type, bool iface);

		
		
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



// Copyright (C) 2016-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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