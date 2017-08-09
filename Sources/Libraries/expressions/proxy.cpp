// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "proxy.h"
#include "array.h"
#include "holder.h"
#include "property.h"
#include "scopeNames.h"

#include "defaultLogs/library.include.h"

namespace Expressions
{

Proxy::Proxy(PathElement* element) 
{
	addPathElement(element);
}

void Proxy::addPathElement(PathElement* element)
{
	path.emplace_back(element);
}

const Expression* Proxy::evaluated(const ScopeNames& environment, boost::any* userData) const
{
	evaluatedValue = path.evaluate(environment);
	
	return evaluatedValue;
}

bool Proxy::canResolveReverence(const ScopeNames& rootEnvironment) const 
{
	return path.canResolveReverence(rootEnvironment);
}

std::string Proxy::string() const
{
	str::stringize result("proxy: ", path.string());

	return result;
}

References Proxy::references() const
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


const Expression* Proxy::Path::evaluate(const ScopeNames& rootEnvironment, boost::any* userData) const
{
	ENFORCE_MSG(!rootEnvironment.empty(), " empty environment.");
	
	const Expression* result = 0;
	const Expression* reference = 0;	
	
	for (auto& element : *this)
	{
		reference = element->evaluate(reference, rootEnvironment);
		if (!reference)
		{
			LOG_ERROR("Proxy can't evaluate path: " << element->string());
			break;
		}
	}

	return reference ? reference->evaluated(rootEnvironment, userData) : 0;
}

bool Proxy::Path::canResolveReverence(const ScopeNames& rootEnvironment) const
{
	const Expression* result = 0;
	const Expression* reference = 0;

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

std::string Proxy::Path::string() const
{
	str::stringize result(str::delimeter("."));
	for (auto& element : *this)
	{
		result << element->string();
	}

	return result;
}


const Expression* PropertyPath::evaluate(const Expression* previous, const ScopeNames& context) const
{		   
	
	
	

	const Expression* res = nullptr;
	if (previous)
	{
		res = previous->child(this);
	}
	else
	{
		res = context.get(name);
	}	 	

	
	
	
	
	

	return res;
}


const Expression* ArrayPath::evaluate(const Expression* input, const ScopeNames& context) const
{
	const Expression* result = input->child(this);
	ENFORCE_MSG(result, input->string() + " is not an array");
	
	return result;
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