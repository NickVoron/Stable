// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "property.h"
#include "reference.h"
#include "holder.h"
#include "namescope.h"

namespace Expressions
{
			 
PropertiesStruct::PropertiesStruct(const std::string& typeName) :
	structType(typeName)
{
	
}

EvaluationUnit* PropertiesStruct::evaluated(EvaluatedScope& environment) const
{
	auto result = Expressions::add<PropertiesStructUnit>(environment, *this);
	auto& scope = result->scope();
	for (auto& iter: *this)
	{
		auto element = iter.second->evaluated(scope);
		scope.add(iter.first, element, InsertMethod::INSERT);
	}

	return result;
}


PropertiesStructUnit::PropertiesStructUnit(Expressions::EvaluatedScope& parent, const PropertiesStruct& prototype_):
	EvaluationUnit("eval_prperties_struct", prototype_, parent),
	prototype(prototype_)
{

}

std::string PropertiesStructUnit::typeName() const
{
	return prototype.structType;
}

std::string PropertiesStructUnit::string() const
{ 
	return str::stringize(typeName()); 
}


void PropertiesStructUnit::add(const std::string& name, EvaluationUnit* value)
{
	scope().add(name, value, InsertMethod::INSERT);
}

const EvaluationUnit* PropertiesStructUnit::get(const std::string& name) const
{
	auto it = scope().find(name);
	return it != scope().end() ? it->second : nullptr;
}

}



// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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