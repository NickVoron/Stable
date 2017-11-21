#include "033.namedSpawners.h"
#include "commonComponents/library.include.h"
#include "utils.h"

#ifdef ENABLE_TEST
namespace ComponentModelTesting
{
	class SpawnerSelector : public UpdatableComponentAutoLink<SpawnerSelector>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(SpawnerSelector);

		void update(){}
	};

	NamedSpawners::NamedSpawners()
	{
		ComponentModel::components<SpawnerSelector>();
		return;

		try
		{
			std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/033.namedSpawners.desc");
			ComponentModel::descriptionTest(path.c_str());
		}
		catch (std::exception& e)
		{
			LOG_ERROR(e);
		}
	}

}//
#endif