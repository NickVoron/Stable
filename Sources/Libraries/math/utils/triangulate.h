#pragma once

#include <vector>
#include "../base/Vector3.h"

namespace Base
{
	namespace MathUtils
	{
		void triangulate(const std::vector<Vector3>& points, std::vector<Vector3>& output);
	}
}


