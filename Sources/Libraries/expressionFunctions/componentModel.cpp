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
					LOG_MSG(val);
				}
			}
		}

		return 0;
	}
}