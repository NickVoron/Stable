// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "020.circlePoints.h"

#ifdef ENABLE_TEST
namespace ComponentModelTesting
{
	
	UserStruct::Vector3List formationCircle(std::size_t count, float circleRadius, float constVal, decltype(&Vector3::x) v0, decltype(&Vector3::x) v1, decltype(&Vector3::x) cv)
	{
		UserStruct::Vector3List result;
		result.resize(count);
		float segmentAngle = nm::TwoPI / count;
		for (std::size_t i = 0; i < count; ++i)
		{
			float a = i * segmentAngle;

			auto& pos = result[i];
			pos.*v0 = sinf(a) * circleRadius;
			pos.*v1 = cosf(a) * circleRadius;
			pos.*cv = constVal;
		}

		return result;
	}

	UserStruct::Vector3List formationCircleXZ(std::size_t count, float circleRadius, float y)
	{
		return formationCircle(count, circleRadius, y, &Vector3::x, &Vector3::z, &Vector3::y);
	}

	
	CirclePoints::CirclePoints()
	{
		BIND_EXPRESSION_FUNCTION(formationCircleXZ);

		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/020.circlePoints.desc");

		
		ObjectParser::Compiler comp(path.c_str());

		testClassesCount(comp.result, 3);

		Expressions::EvaluatedScope worldScopename = unroll(comp.result.classes(), "Main", "main");

		auto superCircle = transform_vec(formationCircleXZ(30, 50, 0), [](std::size_t index, auto flt) { return std::make_tuple(index, flt); });
		auto circle = transform_vec(formationCircleXZ(6, 10, 0), [](std::size_t index, auto flt) { return std::make_tuple(index, flt); });

		for (const auto& superPointTuple: superCircle)
		{
			for (const auto& pointTuple : circle)
			{
				
				std::string path = str::stringize("main.superCircle[", std::get<0>(superPointTuple), "].circle[", std::get<0>(pointTuple), "].transform.position");
				const Expressions::Struct* pos = get(worldScopename, path)->cast<Expressions::Struct>();

				Vector3 resultPoint = std::get<1>(superPointTuple) + std::get<1>(pointTuple);
				test(pos, resultPoint);
			}
		}
	}
}
#endif



// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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