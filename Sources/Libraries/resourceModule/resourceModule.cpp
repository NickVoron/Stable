#include "resourceModule.h"

#include "componentModel/library.include.h"
#include "componentModelConfigurator/library.include.h"

namespace Resources
{
	void Module::Clear()
	{
		deactivate();

		nativeResource.clear();
	}

	void Module::Compile(stream::ostream& os)
	{
		nativeResource.save(os);
	}

	void Module::LoadSource(const char* sourceFile, const UserData* userData)
	{
		ComponentModel::descriptionLoad(sourceFile, nativeResource).activate(true);
		
// 		std::list<boost::filesystem::path> dep;
// 		for(const boost::filesystem::path& d : dep)
// 		{
// 			dependencies.addActualDependence(d, d);
// 		}
	}

	void Module::LoadCompiled(stream::istream& is)
	{
		nativeResource.load(is);
		activate();
	}

	void Module::OnLoadCompiledComplete()
	{
		nativeResource.activate(true);
	}

	void Module::execute()
	{
		resource([](auto& res) { res.execute(); });
	}

}
