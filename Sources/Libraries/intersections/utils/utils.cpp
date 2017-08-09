// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "utils.h"
#include "../shapes/aabb.h"

namespace intersections
{
	void convert(const Quaternion& orientation, nm::Vector3* axis)
	{
		orientation.ToAxes(*(Vector3*) axis[0], *(Vector3*) axis[1], *(Vector3*) axis[2]);
	}


	void convert(const nm::Vector3 axis[3], Quaternion& orientation)
	{
		orientation = orientation.FromAxes(axis[0], axis[1], axis[2]);

	}


	void computeBoundBox(const nm::Vector3* vertices, unsigned int size, intersections::AABB& bb)
	{
		if (size > 0)
		{
			auto min = vertices[0];
			auto max = vertices[0];

			for (unsigned int i = 1; i < size; ++i)
			{
				auto& pos = vertices[i];

				if (min.x > pos.x) min.x = pos.x;
				if (min.y > pos.y) min.y = pos.y;
				if (min.z > pos.z) min.z = pos.z;

				if (max.x < pos.x) max.x = pos.x;
				if (max.y < pos.y) max.y = pos.y;
				if (max.z < pos.z) max.z = pos.z;
			}

			bb.bmin.x = min.x;
			bb.bmin.y = min.y;
			bb.bmin.z = min.z;

			bb.bmax.x = max.x;
			bb.bmax.y = max.y;
			bb.bmax.z = max.z;
		}
	}


	AABB aabb(const Sphere& sphere)
	{
		AABB box;
		box.bmin = sphere.center - nm::Vector3(sphere.radius, sphere.radius, sphere.radius);
		box.bmax = sphere.center + nm::Vector3(sphere.radius, sphere.radius, sphere.radius);
		return box;
	}

	AABB aabb(const nm::Vector3& position, float extent)
	{
		AABB box;
		box.bmin = position - nm::Vector3(extent, extent, extent);
		box.bmax = position + nm::Vector3(extent, extent, extent);
		return box;
	}
}



// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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