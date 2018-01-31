// Copyright (C) 2015-2017 Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "../shapes/shapes.h"
#include "../shapes/obb.h"
#include "../shapes/triangle.h"

namespace intersections
{

	bool test(const AABB& aabb0, const AABB& aabb1);

	bool test(const Ray& ray, const AABB& obb, Vector3& point);
	bool test(const Ray& ray, const OBB& obb);
	bool test(const Ray& ray, const OBB& obb, Vector3& point);
	bool test(const Ray& ray, const Sphere& sphere);
	bool test(const Ray& ray, const Sphere& sphere, Vector3& point);
	bool test(const Ray& ray, const Quad& quad);
	bool test(const Ray& ray, const Triangle& trg);
	bool test(const Segment3& ray, const Triangle& trg);

	bool test(const Ray& ray, const Triangle& trg, Vector3& point);
	bool test(const Ray& ray, const Quad& quad, Vector3& point);
	bool test(const Segment3& ray, const Triangle& trg, Vector3& point);

	template<int vcount>
	bool test(const Ray& ray, const Prysm<vcount>& prysm)
	{
		return true;
	}


}//



// Copyright (C) 2015-2017 Denis Netakhin <denis.netahin@yandex.ru>
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