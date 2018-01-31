// Copyright (C) 2015-2016 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "quad.h"


namespace intersections
{
	struct OBB;

	struct AABB
	{
		AABB() { collapse(); }

		nm::Vector3 bmin;
		nm::Vector3 bmax;

		nm::Vector3 center() const 	{ return bmin + size() * 0.5f; }
		nm::Vector3 size() const { return bmax - bmin; }

		void collapse();

		AABB operator+(const AABB& a2);
		void push(const nm::Vector3& point);
		
			
		void combine(const AABB& aabb);
		

		nm::plane planeXn() const { return nm::plane(bmin, bmin + nm::Vector3(0, 1, 0), bmin + nm::Vector3(0, 0, 1)); }
		nm::plane planeXp() const { return nm::plane(bmax, bmax + nm::Vector3(0, 1, 0), bmax + nm::Vector3(0, 0, 1)); }
		nm::plane planeYn() const { return nm::plane(bmin, bmin + nm::Vector3(1, 0, 0), bmin + nm::Vector3(0, 0, 1)); }
		nm::plane planeYp() const { return nm::plane(bmax, bmax + nm::Vector3(1, 0, 0), bmax + nm::Vector3(0, 0, 1)); }
		nm::plane planeZn() const { return nm::plane(bmin, bmin + nm::Vector3(0, 1, 0), bmin + nm::Vector3(1, 0, 0)); }
		nm::plane planeZp() const { return nm::plane(bmax, bmax + nm::Vector3(0, 1, 0), bmax + nm::Vector3(1, 0, 0)); }

		Quad quadXn() const { return Quad(nm::Vector3(bmin.x, bmin.y, bmin.z), nm::Vector3(bmin.x, bmax.y, bmin.z), nm::Vector3(bmin.x, bmax.y, bmax.z), nm::Vector3(bmin.x, bmin.y, bmax.z)); }
		Quad quadXp() const { return Quad(nm::Vector3(bmax.x, bmin.y, bmin.z), nm::Vector3(bmax.x, bmax.y, bmin.z), nm::Vector3(bmax.x, bmax.y, bmax.z), nm::Vector3(bmax.x, bmin.y, bmax.z)); }

		Quad quadYn() const { return Quad(nm::Vector3(bmin.x, bmin.y, bmin.z), nm::Vector3(bmax.x, bmin.y, bmin.z), nm::Vector3(bmax.x, bmin.y, bmax.z), nm::Vector3(bmin.x, bmin.y, bmax.z)); }
		Quad quadYp() const { return Quad(nm::Vector3(bmin.x, bmax.y, bmin.z), nm::Vector3(bmax.x, bmax.y, bmin.z), nm::Vector3(bmax.x, bmax.y, bmax.z), nm::Vector3(bmin.x, bmax.y, bmax.z)); }

		Quad quadZn() const { return Quad(nm::Vector3(bmin.x, bmin.y, bmin.z), nm::Vector3(bmax.x, bmin.y, bmin.z), nm::Vector3(bmax.x, bmax.y, bmin.z), nm::Vector3(bmin.x, bmax.y, bmin.z)); }
		Quad quadZp() const { return Quad(nm::Vector3(bmin.x, bmin.y, bmax.z), nm::Vector3(bmax.x, bmin.y, bmax.z), nm::Vector3(bmax.x, bmax.y, bmax.z), nm::Vector3(bmin.x, bmax.y, bmax.z)); }

		bool operator==(const AABB& aabb) const;
		void transform(const State& state, float scale);

	};

}//



// Copyright (C) 2015-2016 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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