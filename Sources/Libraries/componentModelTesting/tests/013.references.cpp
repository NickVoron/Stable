#include "013.references.h"
#include "utils.h"

#ifdef ENABLE_TEST
namespace ComponentModelTesting
{

	using namespace Expressions;

	References::References()
	{
		using namespace Expressions;
		using namespace ObjectParser;

		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/013.references.desc");

		//parser test
		ObjectParser::Compiler comp(path.c_str());

 		testClassesCount(comp.result, 4);
		
		//urollTest
		auto worldScopename = unroll(comp.result.classes(), "Main", "main");
		testInstance(worldScopename, "main");		

		auto refsArrayBlue = get(worldScopename, "main.rUtilizer.refs[0]");
		auto refsArrayRed = get(worldScopename, "main.rUtilizer.refs[1]");

		auto mainData = get(worldScopename, "main.data")->cast<ArrayContainer>();

		auto dataCount = mainData->count();
		for (std::size_t i=0; i < dataCount; ++i)
		{
			std::string tRedPath = str::stringize("main.trees[", i, "].red");
			std::string tBluePath = str::stringize("main.trees[", i, "].blue");

			auto tRed = get(worldScopename, tRedPath);
			auto tBlue = get(worldScopename, tBluePath);

//			auto tRedFromArray = refsArrayRed->element(i);
//			auto tBlueFromArray = refsArrayBlue->element(i);
			
//			ENFORCE_MSG(tRed == tRedFromArray, "");
//			ENFORCE_MSG(tBlue == tBlueFromArray, "");

		}

		std::vector<const Expressions::EvaluationUnit*> leafComponents;
		for (int i = 0; i < dataCount; ++i)
		{
			int leafCount = get(worldScopename, str::stringize("main.trees[", i, "].leafCount"))->cast<const Expressions::Const<int>>()->value;

			for (int j=0; j <leafCount; j++)
			{
				auto leaf = get(worldScopename, str::stringize("main.trees[", i, "].leaves[",j,"].leafComponent"));
				leafComponents.push_back(leaf);
			}
		}

		ENFORCE_MSG(leafComponents.size() == 19, "");
		const ArrayContainer* leafArray = get(worldScopename, str::stringize("main.trees.leaves.leafComponent"))->cast<ArrayContainer>();
		ENFORCE_MSG(leafArray, "");
		
		for (size_t i=0; i< leafComponents.size(); i++ )
		{
			ENFORCE_MSG(leafArray->element(i) == leafComponents.at(i), "");
		}
	}

}//
#endif
