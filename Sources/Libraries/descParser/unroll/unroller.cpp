// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "unroller.h"
#include "instanceHandle.h"

#include "../classDescEtc/propertyAssignment.h"
#include "../classDescEtc/expressions/noValueExpression.h"

#include "expressions/library.include.h"


namespace ObjectParser
{

using namespace Expressions;

Unroller::Unroller(const ClassTable& classes_):	classes(classes_)
{

}

Expressions::EvaluationUnit* Unroller::unrollInstance(const InstanceDefinitionExpression& instanceDefinition, Expressions::EvaluatedScope& parentScopenames)
{
	parentScopenames.userData = this;
	EvaluationUnit* unit = instanceDefinition.evaluated(parentScopenames);

	EvaluateState evalState = Reject;
	while (evalState != Complete)
	{
		evalState = unit->evaluateStep(parentScopenames);

		if (evalState == Impossible)
		{
			volatile int i = 0;
		}

		ENFORCE(evalState != Impossible);
	}

	return unit->cast<InstanceHandle>();
}


Expressions::EvaluatedScope unroll(const ClassTable& classTable, const InstanceDefinitionExpression& instance)
{
	ClassDesc* mainClass = classTable.get(instance.type);
	ENFORCE(mainClass);

	Expressions::EvaluatedScope worldScopename;
	Unroller unroller(classTable);
	worldScopename.add("main", unroller.unrollInstance(instance, worldScopename), InsertMethod::INSERT);
	return worldScopename;
}

Expressions::EvaluatedScope unroll(const ClassTable& classTable, const std::string& mainInstanceClassName, const std::string& mainInstanceName)
{
	return unroll(classTable, InstanceDefinitionExpression(mainInstanceClassName, mainInstanceName));
}

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