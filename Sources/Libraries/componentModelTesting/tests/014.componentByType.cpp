// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "014.componentByType.h"


#ifdef ENABLE_TEST
namespace ComponentModelTesting
{
	using namespace Expressions;
	using namespace ObjectParser;

	ComponentByType::ComponentByType()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/014.componentByType.desc");

		
		ObjectParser::Compiler comp(path.c_str());

		testClassesCount(comp.result, 3);
		
		auto worldScopename = unroll(comp.result.classes(), "Main", "main");

		auto objects = get(worldScopename, "main.object")->cast<ArrayContainer>();
		auto childPositionsCandidate = get(worldScopename, "main.manager.draw.childs");
		auto childPositions = childPositionsCandidate->cast<ArrayContainer>();

		ENFORCE_MSG(objects->count() == childPositions->count(), "");
		for (std::size_t i = 0; i < objects->count(); i++)
		{
			const EvaluationUnit* object = objects->element(i);
			




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