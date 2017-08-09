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

namespace ObjectParser
{
	static bool debugGC = false;

	class ExtensionTable;
	class Compiler;

	class GrammarComposition
	{
		friend class Constructor;
		friend class Compiler;
		friend class Parser;
		friend class Lexer;

	public:
		GrammarComposition(Compiler& compiler, ExtensionTable& extensions);
		~GrammarComposition();

		
		
		template<class T>
		Expressions::Const<T>* newConst(const T& value)
		{
			return Expressions::add< Expressions::Const<T> >(value);
		}
	
		
		Expressions::Function* newFunction(const std::string& name, const Expressions::ConstExprList& params);
		Expressions::TernaryOperator* newTernaryOperator(Expressions::Expression* op0, Expressions::Expression* op1, Expressions::Expression* op2);
		Expressions::Array* newArray(const std::unique_ptr<Expressions::ConstExprList>& params);
		Expressions::Struct* newStruct(const std::string& type, const Expressions::ConstExprList& params);
		Expressions::Lambda* newLambda(const std::string& lambdaName, const Expressions::ConstExprList& params);
		Expressions::PropertiesStruct* newPropertiesStruct(const std::string& name, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignment);
		Expressions::Expression* newFunctionOrStruct(const std::string& name, const std::unique_ptr<Expressions::ConstExprList>& params);
		Expressions::ExternalExtension* addExternalExtension(const std::string& extensionName, const std::string& text);

		
		InstanceDefinitionExpression* newPrototype(const std::string& typeName, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList);

		
		void newInstance(const std::string& typeName, const std::string& name, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList);
		void newInstanceArray(const std::string& typeName, const std::string& name, Expressions::Expression* arrayExpr, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList);

		
		Component* newComponent(const std::string& type, const std::string& name);
		Component* newComponent(const std::string& type, const std::string& name, const std::unique_ptr<ComponentParams>&  params);

		Component* addComponent(Component* component, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList);

		void endComponentRuntimeParams(const std::unique_ptr<ComponentParams>& params);

		
		Expressions::Proxy* newProxy();
		Expressions::Proxy* newProxy(const std::string& targetName);

		Expressions::Proxy::PathElement* newProxyArrayPath(int index);
		Expressions::Proxy::PathElement* newProxyPropertyPath(const std::string& name);
		Expressions::Proxy::PathElement* newProxyComponentPath(const std::string& name);

		
		
		void addvar(const std::string& name, Expressions::Expression* expr);

		
		void addProperty(const std::string& name);
		void addProperty(const std::string& name, Expressions::Expression* defaultExpr, bool isPrivate = false);

		
		PropertyAssignmentList*		newPropertyAssignmentsList(PropertyAssignment* assignment);
		PropertyAssignment*			newPropertyAssignment(const std::string& name, Expressions::Expression* value);
		PropertyAssignmentList*		addPropertyAssignments(PropertyAssignmentList* propertyAssignList, PropertyAssignment* assignment);

		
		void bindRuntimeProperty(const std::string& paramName, Expressions::Proxy* path, RuntimeDirection direction);

		
		void setClassName(const std::string& type, bool iface);


		
		
		void addMixInheriatance(const std::string& type, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList);

		
		void addAggregateInheritance(const std::string& type);
		void setInheritanceAssigment(const std::string& type, const std::unique_ptr<PropertyAssignmentList>&  propertyAssignmentList); 
		
		
		
		void endCollection(const std::string& collectionName, const std::string& typeName);
		void addCollectionElement(const std::string& name, Expressions::ConstExprList* guidList);

		const ClassTable& classes() const;

		ExtensionTable& extensions;
		Compiler& compiler;

		void beginClass();
		void endClass();
	private:
		ClassDesc& currentClass();
		
		CollectionExpression* createCollection(const std::string& collectionName, const std::string& typeName);

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