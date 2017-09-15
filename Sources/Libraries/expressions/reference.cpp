// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "reference.h"
#include "array.h"
#include "holder.h"
#include "property.h"
#include "scopeNames.h"

#include "reflection/library.include.h"

namespace Expressions
{

Reference::Reference(PathElement* element) 
{
	addPathElement(element);
}

void Reference::addPathElement(PathElement* element)
{
	path.emplace_back(element);
}

EvaluationUnit* Reference::evaluated(const EvaluatedScope& environment, boost::any* userData) const
{
	return path.evaluate(environment);
}

bool Reference::canResolveReverence(const EvaluatedScope& rootEnvironment) const
{
	return path.canResolveReverence(rootEnvironment);
}

std::string Reference::string() const
{
	str::stringize result("proxy: ", path.string());

	return result;
}

References Reference::references() const
{
	if (path.size())
	{
		if (auto property = dynamic_cast<PropertyPath*>(path[0].get()))
		{
			if (property->name == "iterator" || property->name == "index")
			{
				
				return References();
			}
		}
	}
	References refs;
	refs.push_back(this);
	return refs;
}


EvaluationUnit* Reference::Path::evaluate(const EvaluatedScope& rootEnvironment, boost::any* userData) const
{
	ENFORCE_MSG(!rootEnvironment.empty(), __FUNCTION__" empty environment.");
	
	const EvaluationUnit* result = 0;
	const EvaluationUnit* reference = 0;
	
	for (auto& element : *this)
	{
		reference = element->evaluate(reference, rootEnvironment);
		if (!reference)
		{
			LOG_ERROR("Proxy can't evaluate path: " << element->string());
			break;
		}
	}

	return const_cast<EvaluationUnit*>(reference); 
}

bool Reference::Path::canResolveReverence(const EvaluatedScope& rootEnvironment) const
{
	const EvaluationUnit* result = 0;
	const EvaluationUnit* reference = 0;

	for (auto& element : *this)
	{
		reference = element->evaluate(reference, rootEnvironment);
		if (!reference)
		{
			return false;
		}
	}

	return true;
}

std::string Reference::Path::string() const
{
	str::stringize result(str::delimeter("."));
	for (auto& element : *this)
	{
		result << element->string();
	}

	return result;
}

std::string Reference::Path::root() const
{
	ENFORCE(size() > 0);
	auto& rootPath = at(0);
	
	PropertyPath* rootProperty = dynamic_cast<PropertyPath*>(rootPath.get());
	ENFORCE(rootProperty);

	return rootProperty->name;
}


EvaluationUnit* PropertyPath::evaluate(const EvaluationUnit* previous, const EvaluatedScope& context) const
{		   
	EvaluationUnit* res = nullptr;
	if (previous)
	{
		res = const_cast<EvaluationUnit*>(previous->child(this));
		if (!res)
		{
			auto typeName = previous->typeName();
			auto type = mirror::runtime::type(typeName);
			if (type)
			{
				if (auto structType = previous->cast<Expressions::Struct>())
				{
					auto& params = structType->params;
					auto index = type->index(name);
					if (index.first && index.second < params.size())
					{
						res = (EvaluationUnit*)(params[index.second]);
					}
				}
			}
		}
	}
	else
	{
		res = context.get(name);
	}	 	

	
	
	
	
	

	return res;
}

std::unique_ptr<Reference::PathElement> PropertyPath::copy() const
{
	return std::unique_ptr<Reference::PathElement>(new PropertyPath(name));
}


EvaluationUnit* ArrayPath::evaluate(const EvaluationUnit* input, const EvaluatedScope& context) const
{
	EvaluationUnit* result = const_cast<EvaluationUnit*>(input->child(this));
	ENFORCE_MSG(result, input->string() + " is not an array");
	
	return result;
}


std::unique_ptr<Reference::PathElement> ArrayPath::copy() const
{
	return std::unique_ptr<Reference::PathElement>(new ArrayPath(index));
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