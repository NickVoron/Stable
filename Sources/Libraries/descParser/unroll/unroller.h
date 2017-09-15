// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once


#include <map>
#include <string>

#include "../classDescEtc/grammarComposition.h"
#include "componentModelConfigurator.h"
#include "../classDescEtc/expressions/instanceExpression.h"


namespace ObjectParser
{
	class Unroller
	{
	public:
		Expressions::EvaluationUnit* unrollInstance(const InstanceDefinitionExpression& prototype, Expressions::EvaluatedScope& parentScopenames);
	
		const ClassTable& classes;

		Unroller(const ClassTable& classes, ComponentModelConfigurator& configurator, bool logging);
	
	private:
		ComponentModelConfigurator* configurator;
		Logger logger;
	};

	template<class Configurator>
	class UnrollerT: public Unroller
	{
	public:
		UnrollerT(const ClassTable& classes) :
			Unroller(classes, configurator, false){}

	private:
		Configurator configurator;
	};

	typedef UnrollerT<ComponentModelConfigurator> FinalUnroller;

	Expressions::EvaluatedScope unroll(const ClassTable& classTable, const InstanceDefinitionExpression& instance);
	Expressions::EvaluatedScope unroll(const ClassTable& classTable, const InstanceDefinitionExpression& instance, ComponentModelConfigurator& configurator);
	Expressions::EvaluatedScope unroll(const ClassTable& classTable, const std::string& mainInstanceClassName, const std::string& mainInstanceName);
	Expressions::EvaluatedScope unroll(const ClassTable& classTable, ComponentModelConfigurator& configurator, const std::string& mainInstanceClassName, const std::string& mainInstanceName);
}//



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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