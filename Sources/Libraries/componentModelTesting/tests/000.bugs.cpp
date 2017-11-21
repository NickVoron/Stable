#include "000.bugs.h"
#include "utils.h"


#include "componentModel/library.include.h"
#include "commonComponents/library.include.h"

#ifdef ENABLE_TEST
namespace ComponentModelTesting
{

Bugs::Bugs()
{
	try
	{
		auto files = Base::FileUtils::GetFileNamesByMask(Resources::resourceRelativePath("desc/bugs/"), "desc", true);

		for (auto& path : files)
		{
			ObjectParser::Compiler comp(path.string().c_str());
			auto worldScopename = unroll(comp.result.classes(), "Main", "main");
			ComponentModel::descriptionTest(path.string().c_str());
		}		
	}
	catch (const std::exception& e)
	{
		LOG_ERROR(e.what());
	}
}

}//
#endif