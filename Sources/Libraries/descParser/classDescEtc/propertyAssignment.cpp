// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "propertyAssignment.h"
#include "expressions/library.include.h"


namespace ObjectParser
{

	PropertyAssignment::PropertyAssignment(const std::string& propertyName_, const Expressions::Expression* value_):
		propertyName(propertyName_),
		value(value_)
	{


	}

	bool PropertyAssignment::canResolveReference(const Expressions::EvaluatedScope& parentScopename) const
	{
		ENFORCE_MSG(value, "");
		Expressions::References refs = value->references();
		return refs.canResolveReference(parentScopename);
	}

	
	Expressions::References PropertyAssignmentList::references() const
	{
		Expressions::References refs;
		for (auto& iter : *this)
		{
			const Expressions::References& exprRefs = iter->value->references();
			refs.insert(refs.begin(), exprRefs.begin(), exprRefs.end());
		}

		return refs;
	}

	bool PropertyAssignmentList::exist(const std::string& name) const
	{
		auto& it = find_if(begin(), end(), [&name](auto& element)
		{
			return element->propertyName == name;
		});

		return it != end();
	}

	
	void PropertyAssignmentController::push()
	{
		emplace_back();
	}

	void PropertyAssignmentController::pop()
	{
		if (size())
		{
			pop_back();
		}
	}

	PropertyAssignmentList PropertyAssignmentController::current()
	{
		if (size())
		{
			return back();
		}

		return PropertyAssignmentList();
	}




}//



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