// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once 
#include "component.h"
#include "inheritance/mix.h"

#include "expressions/library.include.h"
#include "stuff/dependencyGraph.h"

#include "expressions/instanceExpression.h"
#include "expressions/componentRefConverter.h"
#include "expressions/constantProperty.h"

namespace ObjectParser
{
	
	class ClassDesc : public Base::NonCopyable
	{
	public:
		
		Expressions::Expression* addProperty(const std::string& name, Expressions::Expression* propert);
		void addConstant(const std::string& name, Expressions::Expression* expr);

		
		Component* addComponent(Component* component);
		Component* addComponent(Component* component, const PropertyAssignmentList& attributes);
		
		
		void addMixInheritance(const std::string& type, const PropertyAssignmentList& params);

		
		void addInstance(InstanceDefinitionExpression* instance);
		void addPrototype(InstanceDefinitionExpression* prototype);

		void addCallable(Expressions::Callable* callable);
		void resolveCallables();

		
		const Expressions::Properties<Expressions::Expression>& properties() const { return _properties; }

		std::string typeName;

		std::vector<std::unique_ptr<MixInheritance>> mixInheritanceList;

		bool isInterface = false;

	private:
		Expressions::Properties<Expressions::Expression> _properties;
		std::vector<Expressions::Callable*> callables;
	};

	struct ClassTable : public std::unordered_map<std::string, std::unique_ptr<ClassDesc>>
	{
		ClassDesc* get(const std::string& type) const;
	};
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