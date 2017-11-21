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