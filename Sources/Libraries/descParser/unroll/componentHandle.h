// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "expressions/library.include.h"
#include "../classDescEtc/propertyAssignment.h"
#include "../classDescEtc/component.h"

namespace ObjectParser
{
	class ComponentHandle : public Expressions::EvaluationUnit
	{
	public:
		ComponentHandle(const Expressions::Expression& proto, Expressions::EvaluatedScope& parent):
			  Expressions::EvaluationUnit("component_handle", proto, parent)
			, links("links", nullptr)
		{}

		ComponentHandle(const Expressions::Expression& proto, Expressions::EvaluatedScope& parent, const ComponentParams& runtimeParams, const PropertyAssignmentList& links, const PropertyAssignmentList& params);

		virtual std::string typeName() const { return type; }
		virtual std::string string() const;

		virtual void extract(EvaluationSet& result) override;
		
		
		ComponentParams& runtimeProperties() { return runtimeParamsLarva;  }
		const ComponentParams& runtimeProperties() const { return runtimeParamsLarva; }

		Expressions::EvaluatedScope links;
		std::string type;
		std::string name;
		std::optional<std::size_t> objectIndex;
		std::optional<std::size_t> componentIndex;

	private:
		
		
		
		
		
		ComponentParams runtimeParamsLarva;	
		PropertyAssignmentList linksLarva;	
		PropertyAssignmentList paramsLarva;	
	};

	class ValueAddressDesc : public Expressions::EvaluationUnit
	{
	public:
		ValueAddressDesc(const ComponentHandle& handle, std::size_t shift);

		const ComponentHandle& componentHandle;
		std::size_t componentShift;
	};


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