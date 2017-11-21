#include "020.circlePoints.h"

#include "positionGenerators/library.include.h"

#ifdef ENABLE_TEST
namespace ComponentModelTesting
{
	//CirclePoints
	CirclePoints::CirclePoints()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/020.circlePoints.desc");

		//parser test
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