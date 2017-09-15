// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "expressions/library.include.h"
#include "../classDescEtc/propertyAssignment.h"
#include "componentHandle.h"

namespace ObjectParser
{
	class InstanceHandle: public Expressions::EvaluationUnit, public Expressions::EvaluatedScope
	{
	public:
		InstanceHandle(const std::string& name_) : name(name_) { index = globalIndexCounter(); }
		virtual ~InstanceHandle() {}

		std::string name;
		std::string type;

		PropertyAssignmentList params;
		Expressions::ExpressionScope unEvaluatedPropertyies;

		virtual std::string typeName() const { return "InstanceHandle"; }

		virtual Expressions::EvaluateState evaluateStep(const Expressions::EvaluatedScope& parentScopename, boost::any* userData = 0) override;
		virtual const Expressions::EvaluationUnit* child(const Expressions::PropertyPath* path) const override;

		const ComponentHandle* component(const std::string& name) const;
		ComponentHandle* component(const std::string& name);
		auto components() const { return expressionsByType<ComponentHandle>(); }
		auto children() const { return expressionsByType<InstanceHandle>(); }

	private:

		template<class ExpressionType>
		std::vector<ExpressionType*> expressionsByType() const
		{
			std::vector<ExpressionType*> result;

			for (auto& unit : *this)
			{
				if (auto expr = dynamic_cast<ExpressionType*>(unit.second))
				{
					result.push_back(expr);
				}
			}

			return result;
		}

		Expressions::EvaluateState urollParams(const Expressions::EvaluatedScope& parentScopename, boost::any* userData);
		Expressions::EvaluateState unrollUnEvaluatedProperies(boost::any* userData);
		
		static size_t globalIndexCounter();
		size_t index = 0;
	};

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