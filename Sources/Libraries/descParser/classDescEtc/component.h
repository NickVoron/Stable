// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "propertyAccumulator.h"
#include "propertyAssignment.h"
#include <boost/any.hpp>

namespace ObjectParser
{
	class ComponentHandle;

	
	class ComponentParam
	{
	public:
		ComponentParam(const std::string& type_, const std::string& name_);
		
		std::string type;  
		std::string value;
	};

	struct ComponentParams : public std::vector<std::unique_ptr<ComponentParam>>
	{
		ComponentParams() {}
		ComponentParams(const ComponentParams& params) { operator=(params); }
		ComponentParams& operator=(const ComponentParams& params)
		{
			for (auto& p : params)
			{
				emplace_back(new ComponentParam(p->type, p->value));
			}

			return *this;
		}
	};

	class ClassDesc;
	
	class Component : public Expressions::Expression
	{
	public:
		virtual std::string string() const override;
		virtual Expressions::EvaluationUnit* evaluated(const Expressions::EvaluatedScope& parentScopenames) const override;
		virtual std::string typeName() const override;

		std::string componentType;
		std::string name;

		RuntimePropeties runtimeProps;
		
		PropertyAssignmentList linkParams;
		PropertyAssignmentList propertyAssigments;

		ComponentHandle* bindedComponent = nullptr; 
	};

	struct Components : public std::unordered_map<std::string, Component*>
	{
		void copy(const Components& src);
		Component* addComponent(Component* component);							
		const Component* findComponent(const std::string& name) const;
	};
	
	
	class ComponentBuilder
	{
	public:
		Component* create(const std::string& type, const std::string& name, ComponentParams& linkParams);
	 	void endRuntimeParams(ComponentParams& params);

		PropertyAccumulator runtimePropetyAccum;
	private:
		ComponentParams runtimeParams;
		
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