// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "componentHandle.h"
#include "prototypeHandle.h"
#include "evaluation.h"

namespace ObjectParser
{
using namespace Expressions;

ComponentHandle::ComponentHandle(const Expression& proto, Expressions::EvaluatedScope& parent, const ComponentParams& runtimeParams_, const PropertyAssignmentList& links_, const PropertyAssignmentList& params_):
	ComponentHandle(proto, parent)
{
	runtimeParamsLarva = runtimeParams_;
	linksLarva = links_;
	paramsLarva = params_;

	Expressions::prepareEvaluationUnits(parent, linksLarva, links, false);
	Expressions::prepareEvaluationUnits(parent, paramsLarva, scope(), false);
}

void ComponentHandle::extract(EvaluationSet& result)
{
	EvaluationUnit::extract(result);
	Expressions::extractDependencies(links, result);
}

std::string ComponentHandle::string() const
{
	return str::spaced("component:", type, "name:", name).str();
}




ValueAddressDesc::ValueAddressDesc(const ComponentHandle& handle, std::size_t shift) :
	EvaluationUnit("value_address_desc", handle.protoexpr, EvaluationUnit::commonParent)
	, componentHandle(handle)
	, componentShift(shift) 
{
}

}



// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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