#include "valueOperation.h"

namespace ValueInterfaceComponents
{
	void ValueOperation::Resource::properties(ValueOperation& component)
	{
		property("input", component.input);
		property("output", component.output);
	}

	ComponentLink<ValueOperation>& ValueOperation::invoke()
	{
		output([this](auto& output)
		{
			input([&output](auto& input)
			{
				LOG_EXPRESSION(&input, &output);
			});
		});

		return output;
	}

	//
	//
	//
	void OpSource::Resource::properties(OpSource& component)
	{
		property("value", component.value);
	}

	//
	//
	//
	void OpTarget::Resource::properties(OpTarget& component)
	{
		property("value", component.value);
	}
	

	//
	//
	//
	void OperationsTick::Resource::properties(OperationsTick& component)
	{
		property("operations", component.operations);
	}
	
	void OperationsTick::update()
	{
		operations([](auto& op)
		{
			op.invoke();
		});
	}
}
