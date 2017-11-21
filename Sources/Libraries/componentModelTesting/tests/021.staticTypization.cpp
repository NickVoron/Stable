#include "021.staticTypization.h"

#include "utils.h"
#include "utils.h"


#ifdef ENABLE_TEST
namespace ComponentModelTesting
{
	using namespace Expressions;
	using namespace ObjectParser;


	StaticTypization::StaticTypization()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/021.staticTypization.desc");

		ObjectParser::Compiler comp(path.c_str());
		auto worldScopename = unroll(comp.result.classes(), "Main", "main");

		auto utilizerObjectRef = get(worldScopename, "main.objectUtilizer.utilizer.references[0]");
		auto blueObjectRef = get(worldScopename, "main.blue.ref");

		ENFORCE_MSG(utilizerObjectRef == blueObjectRef, "");

		auto utilizerComponentRef = get(worldScopename, "main.objectUtilizer.utilizer.references[1]");
		auto blueComponentRef = get(worldScopename, "main.blueRef");

		ENFORCE_MSG(utilizerComponentRef == blueComponentRef, "");
	}
}//
#endif