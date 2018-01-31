// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "componentModel.h"

#include "componentModel/library.include.h"

namespace ExpressionFunctions
{
	template<class MapType>
	auto get_sorted_keys(const MapType& source)
	{
		std::vector<typename MapType::key_type> result;
		for (auto& val : source)
		{
			result.push_back(val.first);
		}
		std::sort(result.begin(), result.end());

		return result;
	}

	int component_info(Expressions::Expression* expr)
	{
		if (auto component = expr->cast<ObjectParser::ComponentHandle>())
		{
			auto index = ComponentsFactory::classIndex(component->typeName());
			if (index.second)
			{
				ResourceTable* res = ComponentsFactory::resourceTable(index.first);

				for (auto& val : get_sorted_keys(*res))
				{
					LOG_MSG(val.c_str());
				}
			}
		}

		return 0;
	}
}




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