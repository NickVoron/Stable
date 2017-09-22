// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include <list>

#include "stuff/library.include.h"
#include "expressions/library.include.h"

namespace ObjectParser
{

	
	
	
	
	
	
	
	
	
	
	
	
	
	

	struct PropertyAssignment: public Base::NonCopyable
	{
	public:
		PropertyAssignment(const std::string& propertyName_,const Expressions::Expression* value_);
		std::string propertyName;
		const Expressions::Expression* value = 0;

		bool canResolveReference(const Expressions::EvaluatedScope& parentScopename) const;

	};

	struct PropertyAssignmentList : public std::vector<std::unique_ptr<const PropertyAssignment>>
	{
		PropertyAssignmentList() {}
		PropertyAssignmentList(const PropertyAssignmentList& params) { operator=(params); }

		PropertyAssignmentList& operator=(const PropertyAssignmentList& params) 
		{
			for (auto& p : params)
			{
				emplace_back(new PropertyAssignment(p->propertyName, p->value));
			}

			return *this;
		}

		Expressions::References references() const;
		bool exist(const std::string& name) const;
	};

	struct PropertyAssignmentController : private std::vector<PropertyAssignmentList>
	{
		void push();
		void pop();
		PropertyAssignmentList current();
	};



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