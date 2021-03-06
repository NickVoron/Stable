// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "instanceExpression.h"
#include "../../unroll/unroller.h"
#include "../../unroll/prototypeHandle.h"
#include "../../unroll/instanceHandle.h"
#include "../classDesc.h"

namespace ObjectParser
{

using namespace Expressions;


InstanceDefinitionExpression::InstanceDefinitionExpression(const std::string& type_, const std::string& name_, const PropertyAssignmentList& params_, bool noinst):
	type(type_),
	name(name_),
	params(params_),
	noinstance(noinst)
	{
	}

InstanceDefinitionExpression::InstanceDefinitionExpression(const std::string& type_, const std::string& name_):
	InstanceDefinitionExpression(type_, name_, PropertyAssignmentList(), false)
{
}

std::string InstanceDefinitionExpression::string() const
{
	auto value = str::stringize("InstanceDefinitionExpression type: ", type, ", name: ", name).str();

	if (arrayData)
	{
		value += str::stringize(", arrayData:", arrayData->string()).str();
	}

	return value;
}

InstanceDefinitionExpression* InstanceDefinitionExpression::instance() const
{
	return add<InstanceDefinitionExpression>(type, name, params, false);
}



EvaluationUnit* InstanceDefinitionExpression::evaluated(EvaluatedScope& namescope) const
{
	Unroller* unroller = InstanceHandle::unroller;
	ENFORCE_POINTER(unroller);

	EvaluationUnit* result = nullptr;

	const ClassDesc* instanceClass = unroller->classes.get(type);
	ENFORCE_MSG(instanceClass, str::spaced("class:", type, "not exist"));

	if (!noinstance)
	{
		result = add<InstanceHandle>(*instanceClass, *this, namescope);
	}
	else
	{
		result = add<PrototypeHandle>(namescope, *this, unroller->classes, unroller->entities);
	}

	return result;
}


}



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