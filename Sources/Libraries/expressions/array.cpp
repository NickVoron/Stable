// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "array.h"
#include "holder.h"
#include "defaultLogs/library.include.h"
#include "proxy.h"

namespace Expressions
{

Array::Array(const ConstExprList& params_) : elements(params_)
{	
}

std::string Array::string() const
{
	str::stringize result("array: ", str::comma());

	for (std::size_t i = 0; i < count(); ++i)
	{
		result(stringize(element(i)));
	}

	return result;
}

std::string Array::typeName() const
{
	return "Array"; 
}

References Array::references() const
{
	return elements.references();
}

const Expression* Array::evaluated(const ScopeNames& environment, boost::any* userData) const
{
	Array* result = Expressions::add<Array>();
	for (auto& element: elements)
	{
		result->add(element->evaluated(environment, userData));
	}

	return result;
}

const Expression* Array::child(const PropertyPath* path) const
{
	Array* result = Expressions::add<Array>();
	for (auto& element : elements)
	{
		const Expression* elementChild = element->child(path);
		if (const Array* elementArray = elementChild->cast<Array>())
		{
			for (auto& childArrayElement: elementArray->elements)
			{
				result->add(childArrayElement);
			}
		}
		else
		{
			result->add(elementChild);
		}
	}

	return result;
}

const Expression* Array::child(const ArrayPath* path) const
{
	return element(path->index);
}

const Expression* Array::element(std::size_t index) const
{
	if(index >= elements.size())
	{
		if(!elements.empty())
		{
			LOG_ERROR("index is out of range. index: " << index << " range: (" << 0 << ", " << elements.size() - 1 << ")");
		}
		else
		{
			LOG_ERROR("array is empty");
		}		

		return nullptr;
	}

	return elements[index];
}

std::size_t Array::count() const
{ 
	return elements.size(); 
}

void Array::add(const Expression* expr)
{
	elements.push_back(expr);
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