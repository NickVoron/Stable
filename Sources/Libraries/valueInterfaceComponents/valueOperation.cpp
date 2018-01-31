// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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

	
	
	
	void OpSource::Resource::properties(OpSource& component)
	{
		property("value", component.value);
	}

	
	
	
	void OpTarget::Resource::properties(OpTarget& component)
	{
		property("value", component.value);
	}
	

	
	
	
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




// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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