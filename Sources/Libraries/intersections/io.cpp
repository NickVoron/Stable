// Copyright (C) 2014-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "io.h"

#include "math/library.include.h"

namespace intersections
{
	std::ostream& operator<<(std::ostream& os, const AABB& obj)
	{
		return os << "AABB: min: " << Vector3(&obj.bmin.x) << " max: " << Vector3(&obj.bmax.x);
	}

	std::ostream& operator<<(std::ostream& os, const OBB& obj)
	{
		return os << "OBB: center: " << obj.center << " axis[0]: " << obj.axis[0] << " axis[1]: " << obj.axis[1] << " axis[2]: " << obj.axis[2] << " extent[0]: " << obj.extent[0] << " extent[1]: " << obj.extent[1] << " extent[2]: " << obj.extent[2];
	}

	std::ostream& operator<<(std::ostream& os, const Sphere& obj)
	{
		return os << "SPHERE: center: " << obj.center << " radius: " << obj.radius;
	}

	std::ostream& operator<<(std::ostream& os, const Ray& obj)
	{
		return os << "RAY: origin: " << obj.origin << " direction: " << obj.direction;
	}

	std::ostream& operator<<(std::ostream& os, const Line& obj)
	{
		return os << "LINE: origin: " << obj.origin << " direction: " << obj.direction;
	}

	std::ostream& operator<<(std::ostream& os, const Segment3& obj)
	{
		return os << "SEGMENT: p0: " << obj.p0 << " p1: " << obj.p1;
	}

	std::ostream& operator<<(std::ostream& os, const Capsule3& obj)
	{
		return os << "CAPSULE: segment: " << obj.segment << " radius: " << obj.radius;
	}
}








// Copyright (C) 2014-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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