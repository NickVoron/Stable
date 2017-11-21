#include "029.external.h"
#include "utils.h"


#include "componentModel/library.include.h"
#include "commonComponents/library.include.h"

#ifdef ENABLE_TEST
namespace ComponentModelTesting
{

ExternalLanguage::ExternalLanguage()
{
	std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/029.external.desc");
	ObjectParser::Compiler comp(path.c_str());
	auto worldScopename = unroll(comp.result.classes(), "Main", "main");
}

}//
#endif