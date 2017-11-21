#include "019.crossReference.h"
#include "013.references.h"
#include "utils.h"

namespace ComponentModelTesting
{

class InnerComponent : public UpdatableComponentAutoLink<InnerComponent>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(InnerComponent);

	struct Resource : public ResourceBase
	{
		void properties(InnerComponent& component)
		{
			property("link", component.link);
			property("info", component.info);
		}
	};

	void update()
	{
		link([this](auto& target)
		{
//			LOG_EXPRESSION(info, target.info);
		});
	}

	std::string info;
	ComponentLink<InnerComponent> link;
};
	
CrossReference::CrossReference()
{
	ComponentModel::components<InnerComponent>();

	std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/019.crossReference.desc");
	ObjectParser::Compiler comp(path.c_str());
	auto worldScopename = unroll(comp.result.classes(), "Main", "main");


	auto* aComponent = get(worldScopename, "main.a.aComponent");
	ENFORCE_MSG(aComponent, "main.a.aComponent not exist");

	auto* bComponent = get(worldScopename, "main.b.bComponent");
	ENFORCE_MSG(bComponent, "main.b.bComponent not exist");

	test(worldScopename, "main.b.bComponent.link", aComponent);
	test(worldScopename, "main.a.aComponent.link", bComponent);

 	EntitiesList entities;
 	ComponentModel::descriptionLoad(path, entities).activate(true);
 	entities.execute();
}

}//
