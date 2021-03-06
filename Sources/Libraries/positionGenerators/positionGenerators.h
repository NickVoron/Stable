// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "math/library.include.h"

#include <vector>

namespace Positions
{
	std::vector<Vector3> circleXZ(std::size_t count, Vector3 center, float circleRadius);
	std::vector<Vector3> circleYZ(std::size_t count, Vector3 center, float circleRadius);
	std::vector<Vector3> circleXY(std::size_t count, Vector3 center, float circleRadius);

	std::vector<Vector3> regularXZ(Vector2 minCorner, Vector2 maxCorner, float y, Vector2 step);
	std::vector<Vector3> regularYZ(Vector2 minCorner, Vector2 maxCorner, float x, Vector2 step);
	std::vector<Vector3> regularXY(Vector2 minCorner, Vector2 maxCorner, float z, Vector2 step);
	std::vector<Vector3> regularXYZ(Vector3 minCorner, Vector3 maxCorner, Vector3 step);
}



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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