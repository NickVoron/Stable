#include "026.cyclicSpawners.h"
#include "utils.h"


#include "componentModel/library.include.h"
#include "commonComponents/library.include.h"

#ifdef ENABLE_TEST
namespace ComponentModelTesting
{

CyclicSpawners::CyclicSpawners()
{
	std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/026.cyclicSpawners.desc");
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