#include "componentRefConverter.h"
#include "../../unroll/componentHandle.h"
#include "../component.h"


namespace Expressions
{
	bool convert(Expressions::EvaluationUnit& expr, LinksDescList& client)
	{
//		LOG_WARNING("references need check for compatible types");
		std::vector<ObjectParser::ComponentHandle*> descriptions;
 		linearize(expr, descriptions);
 		auto count = descriptions.size();
 		if (count > 0)
 		{
 			client.resize(count);
 			for (std::size_t i = 0; i < count; ++i)
 			{
 				auto componentHandle = descriptions[i];
				//LOG_EXPRESSION(componentHandle->string(), componentHandle->objectIndex.value(), componentHandle->componentIndex.value());
 				auto& address = client[i].address; 
				if(componentHandle->objectIndex)
				{
					address.objectIndex = componentHandle->objectIndex.value();
				}  				

				if(componentHandle->componentIndex)
				{
					address.componentIndices.push_back(componentHandle->componentIndex.value());
				}				
 			}
 		}

		return true;
	}

	//
	//
	//
	bool convert(Expressions::EvaluationUnit& expr, LinkDesc& client)
	{
		LinksDescList list;
		convertVar(expr, list);
		if (list.size() > 0)
		{
			client = list[0];
		}

		return true;
	}

	bool convert(Expressions::EvaluationUnit& expr, ValueAddress& client)
	{
		if (auto addrexpr = expr.cast<ObjectParser::ValueAddressDesc>())
		{
			if(addrexpr->componentHandle.componentIndex)
			{
				client.componentIndex = addrexpr->componentHandle.componentIndex.value();
				client.componentShift = addrexpr->componentShift;
			}			
		}	

		return client.componentIndex.has_value();
	}
}