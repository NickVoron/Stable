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
//				LOG_INFO(logs::tabs(level) << "prototype: className: " << prototype->prototype.type);
				auto& sourceClasses = prototype->entities.classes;
				EntitiesList entities;
				auto worldScope = ObjectParser::unroll(prototype->classTable, prototype->entities, *prototype->prototype.instance(), (EvaluatedScope*)&expr.scope());

				// выбираем все ссылки на компоненты внешних объектов
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
//				sourceClasses.debugOutput();
//				remapEntitiesToClassesLib(entities, *classes);
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