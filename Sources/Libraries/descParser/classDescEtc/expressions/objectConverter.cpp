// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "objectConverter.h"
#include "../../unroll/prototypeHandle.h"

namespace ComponentModel
{	
	EntitiesList& initializeEntities(const Expressions::EvaluatedScope& scope, EntitiesList& entities, ExternalComponentLinks& externalLinks);
}

namespace Expressions
{
	bool convert(const EvaluationUnit& expr, EntitiesStream& client)
	{
		client = EntitiesStream();

		if (auto prototype = expr.cast<ObjectParser::PrototypeHandle>())
		{
			auto& prototypes = prototype->entities.classes.prototypes;
			void* identifier = (void*)&prototype->prototype;

			if (!prototypes.find(identifier))
			{			 
				auto& proto = prototypes.create(identifier, prototype->prototype.type);
				auto& ios = proto.ios;
				LOG_INFO("prototype class name: " << prototype->prototype.type);
				auto& sourceClasses = prototype->entities.classes;
				EntitiesList entities;
				auto worldScope = ObjectParser::unroll(prototype->classTable, prototype->entities, *prototype->prototype.instance(), (EvaluatedScope*)&expr.scope());

				
				ComponentModel::ExternalComponentLinks externalLinks;
				for(auto& object : *worldScope)
				{
					auto& scope = object.second->scope();
					for(auto& property : scope)
					{
						if (auto handle = property.second->cast<ObjectParser::ComponentHandle>(); handle)
						{
							if (!scope.isClassMember(handle))
							{
								auto& externalLink = externalLinks[handle];
								convertVar(*handle, externalLink.link);
							}
						}											
					}
				}

				std::vector<ComponentExternalLink> links;
				for (auto& link : externalLinks)
				{
					links.push_back(link.second);
				}

				
				ios << links;
				ComponentModel::initializeEntities(*worldScope, entities, externalLinks);
				sourceClasses.merge(entities.classes);


				entities.saveObjects(ios);
			}

			client.streamId = identifier;
		}
		else
		{
			LOG_ERROR("type: " << expr.string() << " is not an object");
		}

		return client.streamId != nullptr;
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