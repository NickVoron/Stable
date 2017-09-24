// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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
#include "function.h"

#include "reflection/library.include.h"

namespace Expressions
{

Reference::Reference(PathElement* element) 
{
	addPathElement(element);
}

Reference::Reference(Expression* head_):
	head(head_)
{

}

void Reference::addPathElement(PathElement* element)
{
	path.add(element);
}

EvaluationUnit* Reference::evaluated(const EvaluatedScope& environment) const
{
	return path.evaluate(environment, head);
}

bool Reference::canResolveReference(const EvaluatedScope& rootEnvironment) const
{
	if(head)
	{
		References refs = head->references();
		return refs.canResolveReference(rootEnvironment);
	}
	
	ENFORCE(!head);

	return path.canResolveReference(rootEnvironment);
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


void Reference::Path::add(PathElement* element)
{
	emplace_back(element);
	path_string = string();
}

EvaluationUnit* Reference::Path::evaluate(const EvaluatedScope& rootEnvironment, Expression* head) const
{
	const EvaluationUnit* result = 0;
	const EvaluationUnit* reference = 0;
	if (head)
	{
		reference = head->evaluated(rootEnvironment);
	}
	
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

bool Reference::Path::canResolveReference(const EvaluatedScope& rootEnvironment) const
{
	const EvaluationUnit* reference = 0;

	for (auto& element : *this)
	{
		reference = element->evaluate(reference, rootEnvironment);
		if (!reference)
		{
			return false;
		}
	}

	if (reference && reference->cast<FunctionUnit>())
	{
		return false;
	}

	return true;
}

std::string Reference::Path::string() const
{
	str::stringize result;
	bool first = true;
	for (auto& element : *this)
	{
		if (auto arrayPath = dynamic_cast<ArrayPath*>(element.get()))
		{
			result(str::nodelim(), "");
		}
		else
		{
			if (!first)
			{
				result(".");
			}
		}

		result(element->string());

		first = false;
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


EvaluationUnit* ArrayPath::evaluate(const EvaluationUnit* input, const EvaluatedScope& context) const
{
	EvaluationUnit* result = const_cast<EvaluationUnit*>(input->child(this));
	ENFORCE_MSG(result, input->string() + " is not an array");	
	return result;
}

}//



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