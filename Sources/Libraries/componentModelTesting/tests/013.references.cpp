// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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

		
		ObjectParser::Compiler comp(path.c_str());

 		testClassesCount(comp.result, 4);
		
		
		EvaluatedScope worldScopename = unroll(comp.result.classes(), "Main", "main");
		testInstance(worldScopename, "main");		

		const EvaluatedArray* refsArrayBlue = get(worldScopename, "main.rUtilizer.refs[0]")->cast<EvaluatedArray>();
		const EvaluatedArray* refsArrayRed = get(worldScopename, "main.rUtilizer.refs[1]")->cast<EvaluatedArray>();

		auto dataCount = get(worldScopename, "main.data")->cast<const Expressions::EvaluatedArray>()->count();
		for (std::size_t i=0; i < dataCount; ++i)
		{
			std::string tRedPath = str::stringize("main.trees[", i, "].red");
			std::string tBluePath = str::stringize("main.trees[", i, "].blue");

			const Expression* tRed = get(worldScopename, tRedPath);
			const Expression* tBlue = get(worldScopename, tBluePath);

			const Expression* tRedFromArray = refsArrayRed->element(i);
			const Expression* tBlueFromArray = refsArrayBlue->element(i);
			
			ENFORCE_MSG(tRed == tRedFromArray, "");
			ENFORCE_MSG(tBlue == tBlueFromArray, "");

		}

		std::vector<const Expressions::Expression*> leafComponents;
		for (int i = 0; i < dataCount; ++i)
		{
			int leafCount = get(worldScopename, str::stringize("main.trees[", i, "].leafCount"))->cast<const Expressions::Const<int>>()->value;

			for (int j=0; j <leafCount; j++)
			{
				const Expressions::Expression* leaf = get(worldScopename, str::stringize("main.trees[", i, "].leaves[",j,"].leafComponent"));
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

}
#endif




// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.