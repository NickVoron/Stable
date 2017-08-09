// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "scopeNames.h"
#include "expression.h"

namespace Expressions
{

const std::string ScopeNames::unnamed = "unnamed";


void ScopeNames::add(const std::string& name, const Expressions::Expression* expr, Method method)
{
	auto found = find(name);
	ENFORCE_MSG(method != INSERT || found == this->end(), "INSERT: scopenames alredy have property by name: " + name);
	ENFORCE_MSG(method != REPLACE || found != this->end(), "REPLACE: scopenames not have property by name: " + name);

	if (method==IGNORE_IF_EXIST && found != end())
	{
		return;
	}

	(*this)[name] = expr;
}

const Expressions::Expression* ScopeNames::get(const std::string& name) const
{
	auto iter = find(name);
	return (iter != end()) ? iter->second : nullptr;
}

bool ScopeNames::exists(const std::string& name) const
{
	return find(name) != end();
}

const Expressions::Expression* ScopeNames::getByType(const std::string& type) const
{
	for (auto& iter: *this)
	{
		const Expression* expr = iter.second;
		if (expr->typeName() == type)
		{
			return expr;
		}
	}

	return 0;
}

void ScopeNames::copyFrom(const ScopeNames& proto)
{
	(map&)(*this) = (map&)proto;
}

}//



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