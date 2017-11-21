#include "io.h"

//#include "componentsInitScript/library.include.h"

namespace ComponentsIO
{
	void save(const char* fileName, EntitiesList& entities)
	{
		stream::ofstream<> os(fileName);
		entities.save(os);
	}

	void load(const char* fileName, EntitiesList& entities)
	{
		if(Base::FileUtils::FileExists(fileName))
		{
			stream::ifstream<> is(fileName);	
			entities.load(is);
			entities.activate(true);
		}
	}

	void load_script(const std::string& source, EntitiesList& entities, const std::string& category, bool createObjects, bool activate)
	{
		INCOMPLETE;
		//ComponentsScript::load(("../../Resources/desc/" + source).c_str(), entities, category, createObjects);

		//if (activate && createObjects)
		//{
		//	entities.activate(true);
		//}		
	}

	Resources::Module* load_script(const std::string& source)
	{
		Resources::Module* loaded = 0;
		Resources::load(loaded, source, false);
		return loaded;
	}
}