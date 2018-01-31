// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "020.circlePoints.h"

#include "positionGenerators/library.include.h"

#ifdef ENABLE_TEST
namespace ComponentModelTesting
{
	
	CirclePoints::CirclePoints()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/020.circlePoints.desc");

		
		ObjectParser::Compiler comp(path.c_str());

		testClassesCount(comp.result, 3);

		auto worldScopename = unroll(comp.result.classes(), "Main", "main");

		auto superCircle = transform_vec(Positions::circleXZ(3, Vector3(0,0,0), 50.0f), [](std::size_t index, auto flt) { return std::make_tuple(index, flt); });
		auto circle = transform_vec(Positions::circleXZ(5, Vector3(0, 0, 0), 10), [](std::size_t index, auto flt) { return std::make_tuple(index, flt); });

		for (const auto& superPointTuple: superCircle)
		{
			for (const auto& pointTuple : circle)
			{				
				std::string path = str::stringize("main.superCircle[", std::get<0>(superPointTuple), "].circle[", std::get<0>(pointTuple), "].transform.position");
				auto pos = get(worldScopename, path);
				
				Vector3 resultPoint = std::get<1>(superPointTuple) + std::get<1>(pointTuple);
				test(*pos, resultPoint);
			}
		}

		ComponentModel::descriptionTest(path.c_str());
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