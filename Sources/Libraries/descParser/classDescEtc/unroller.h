// Copyright (C) 2016-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once


#include <map>
#include <string>

#include "grammarComposition.h"
#include "componentModelConfigurator.h"
#include "expressions/library.include.h"


namespace ObjectParser
{
	class Unroller
	{
	public:
		Unroller(const ClassTable& classes, ComponentModelConfigurator& configurator, bool logging);
	
		
		
		Expressions::Expression* unrollInstance(const InstanceDefinitionExpression* prototype, Expressions::ScopeNames& parentScopenames);
		ComponentHandle* unrollComponent(const Expressions::ScopeNames& instanceScopenames, const Component& component);
	
		const ClassTable& classes;

	private:
		InstanceHandle* unrollObject(const Expressions::ScopeNames& parentScopenames, const InstanceDefinitionExpression& prototype);
		PrototypeHandle* unrollPrototype(const Expressions::ScopeNames& parentScopenames, const InstanceDefinitionExpression& prototype);

		void unrollMixInheriatence(ClassDesc* instanceClass, const MixInheritance& mix);
		void resolveNames(Expressions::ScopeNames& instanceScopenames, const Expressions::ScopeNames& unEvaluatedPropertyies);
		Expressions::ScopeNames fillScopenamesFromClass(Expressions::ScopeNames& result, const ClassDesc& classDesc);
		Expressions::ScopeNames assignInputValues(const PropertyAssignmentList& propertyAssigments, const Expressions::ScopeNames& parentScopenames);
		bool canResolveDependencies(const Expressions::References& refs, Expressions::ScopeNames& scopeName);
	
		struct CurrentTask
		{
			const InstanceDefinitionExpression* prototype = nullptr;
			Expressions::ScopeNames scopenames;
		};

		CurrentTask current;
		ComponentModelConfigurator& configurator;
		Logger logger;
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