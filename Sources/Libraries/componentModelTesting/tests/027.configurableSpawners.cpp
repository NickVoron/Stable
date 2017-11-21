#include "027.configurableSpawners.h"
#include "utils.h"


#include "componentModel/library.include.h"
#include "commonComponents/library.include.h"

#ifdef ENABLE_TEST
namespace ComponentModelTesting
{

class ComponentCS1 : public UpdatableComponentAutoLink<ComponentCS1, ConfigurableSpawner<Position>>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ComponentCS1);

	void update() 
	{ 
 		spawn([](Position& component)
 		{ 
 			LOG_EXPRESSION(typeid(component).name()) 
 		}); 
	}
};

															 
ConfigurableSpawners::ConfigurableSpawners()
{
	ComponentModel::components<ComponentCS1>();

	std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/027.configurableSpawners.desc");
	ObjectParser::Compiler comp(path.c_str());
	

	auto worldScopename = unroll(comp.result.classes(), "Main", "main");

	try
	{
		ComponentModel::descriptionTest(path.c_str());
	}
	catch (const std::exception& e)
	{
		LOG_ERROR(e.what());
	}
}

}//
#endif