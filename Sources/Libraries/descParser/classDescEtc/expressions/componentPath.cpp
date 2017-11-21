#include "componentPath.h"
#include "instanceExpression.h"
#include "stuff/enforce.h"
#include "../../unroll/instanceHandle.h"
#include "expressions/library.include.h"

namespace ObjectParser 
{
	using namespace Expressions;

	//
	// это небольшое нарушение инкапсуляции 
	/*
	Expressions::EvaluationUnit* ComponentPath:: evaluate(Expressions::EvaluationUnit* input, const Expressions::EvaluatedScope& context) const
	{
		Expressions::EvaluationUnit* result = 0;

		if (input)
		{
			if (const InstanceHandle* instace = input->cast<InstanceHandle>() )
			{
				// значит тут не массив, а инстанс
				 result = const_cast<Expressions::EvaluationUnit*>(getComponent(instace, componentType));
			}
			else
			{
				//у нас массив инстансов 
				const Expressions::EvaluatedArray* array = input->cast<const Expressions::EvaluatedArray>();
				ENFORCE_POINTER(array);

				Expressions::EvaluatedArray* resultArr = Expressions::add<Expressions::EvaluatedArray>(input->protoexpr, array->count(), EvaluationUnit::commonParent);
				result = resultArr;

				for (std::size_t i = 0; i < array->count(); ++i)
				{
					const InstanceHandle* instanceHandle = array->element(i)->cast<const InstanceHandle>();
					ENFORCE_MSG(instanceHandle, "");

					const Expressions::EvaluationUnit* foundComponent = getComponent(instanceHandle, componentType);
					if (!foundComponent)
					{
						//если хотя бы один не нашли - то значит не можем вообще ничего выбрать и надо возрващать нуль
						result = 0;
						break;
					}
					resultArr->add(foundComponent);
				}
			}
		}
		else
		{
			LOG_ERROR("context: input is nullptr");
		}

		return result;
	}*/

	Expressions::EvaluationUnit* ComponentPathElement::evaluated(Expressions::EvaluatedScope& namescope) const
	{
		return add<ComponentPathElementUnit>(namescope, *this);
	}		

	Expressions::BaseIndexerUnit* ComponentPathElement::createIndexer(Expressions::EvaluatedScope& namescope) const
	{
		return add<Expressions::SimpleIndexerUnit>(namescope, *this);
	}

	Expressions::BaseSamplerUnit* ComponentPathElement::createSampler(Expressions::EvaluatedScope& namescope) const
	{
		return add<ComponentTypeSamplerUnit>(namescope, *this);
	}

	//
	//
	//
	//
	//
	//
	ComponentTypeSamplerUnit::ComponentTypeSamplerUnit(Expressions::EvaluatedScope& parent, const ComponentPathElement& proto) :
		Expressions::BaseSamplerUnit(parent, proto), element(proto)
	{

	}

	Expressions::EvaluationUnit* ComponentTypeSamplerUnit::sample(Expressions::EvaluationUnit* source) const
	{
		auto instanceHandle = source->cast<InstanceHandle>();
		ENFORCE_POINTER(instanceHandle);
		return instanceHandle->scope().getByType(element.componentType);
	}
}//