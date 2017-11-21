#include "bug0006.h"
#include "../tests/utils.h"
#include "descParser/library.include.h"

namespace ComponentModelTesting
{

	using namespace Expressions;
	using namespace ObjectParser;

	Bug0006::Bug0006()
	{
		std::string path = Resources::resourceRelativePath("desc/bugs/bug0006.desc");

		//parser test
		ObjectParser::Compiler comp(path.c_str());

		std::unique_ptr<Expressions::EvaluatedScope> worldScopename = unroll(comp.result.classes(), "Main", "main");

	
		test(worldScopename, "main.o2.posComp.position", Vector3(5,0,0));
		test(worldScopename, "main.o2.link.position", Vector3(-5, 0, 0));

		test(worldScopename, "main.o3.posComp.position", Vector3(-5, 0, 0));
		auto noVal =  get(worldScopename, "main.o3.link")->cast<VoidExpression>();
		ENFORCE_MSG(noVal, "'main.o3.link' not exist")

	}

}