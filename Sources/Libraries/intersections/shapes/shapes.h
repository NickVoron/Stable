// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <iostream>
#include <algorithm>

#include "math/library.include.h"
#include "newmath/library.include.h"

#include "triangle.h"

namespace intersections
{
	

	struct Sphere 
	{
		static Sphere zero();
		static Sphere one(float x, float y, float z);

		Sphere(){};
		Sphere(const nm::Vector3& c, float r):center(c), radius(r){};
		nm::Vector3 center;
		float radius;

		void merge(const Sphere& sphere);
		void transform(const State& state, float scale);

	};

	struct Ray
	{
		Ray(){}
		Ray(const nm::Vector3& org, const nm::Vector3& dir) : origin(org), direction(dir){}
		nm::Vector3 origin;
		nm::Vector3 direction;
	};


	struct Line : public Ray 
	{
	};

	struct Segment3 
	{
		nm::Vector3 p0;
		nm::Vector3 p1;

		nm::Vector3 direction() const
		{
			return p1 - p0;
		}

	};

	struct Capsule3 
	{
		Segment3 segment;
		float radius;
	};

}




// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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