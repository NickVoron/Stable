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
	class Unroller;
	class InstanceDefinitionExpression;

	class InstanceHandle: public Expressions::EvaluationUnit
	{
	public:
		static Unroller* unroller;
		InstanceHandle(const ClassDesc& classDesc, const InstanceDefinitionExpression& proto, Expressions::EvaluatedScope& parent);
		
		virtual std::string typeName() const override;
		virtual std::string string() const override;

		const ComponentHandle* component(const std::string& name) const;
		ComponentHandle* component(const std::string& name);
		auto components() const { return expressionsByType<ComponentHandle>(); }
		auto children() const { return expressionsByType<InstanceHandle>(); }

		virtual void extract(EvaluationSet& result) override;
		virtual Expressions::EvaluationInfo evaluate() override;
		
		const ClassDesc& classDesc;
		const InstanceDefinitionExpression& definition;
		Expressions::ExpressionScope classProperties;
		EvaluationUnit* evaluatedArrayData = nullptr;
		Expressions::EvaluatedScope localScope;
		
	private:
		template<class ExpressionType>
		std::vector<ExpressionType*> expressionsByType() const
		{
			std::vector<ExpressionType*> result;

			for (auto& unit : internalScope)
			{
				if (auto expr = dynamic_cast<ExpressionType*>(unit.second))
				{
					if (internalScope.isClassMember(expr))
					{
						result.push_back(expr);
					}					
				}
			}

			return result;
		}
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