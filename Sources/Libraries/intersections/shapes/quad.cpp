// Copyright (C) 2016-2017 Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "quad.h"

namespace intersections
{
	Quad Quad::create(const State& state, const Vector2& halfSize)
	{
		Vector3 pos = state.position;

		Quad q;

		q.vertices[0] = pos + state.orientation.Transform(Vector3(-halfSize.x, -halfSize.y, 0));
		q.vertices[1] = pos + state.orientation.Transform(Vector3(halfSize.x, -halfSize.y, 0));
		q.vertices[2] = pos + state.orientation.Transform(Vector3(halfSize.x, halfSize.y, 0));
		q.vertices[3] = pos + state.orientation.Transform(Vector3(-halfSize.x, halfSize.y, 0));

		return q;
	}

	void Quad::transform(const State& state, float scale)
	{
		for (int i = 0; i < VERTICES_COUNT; ++i)
		{
			vertices[i] = state.GetLocalVector(vertices[i] * scale);
		}

	}


}




// Copyright (C) 2016-2017 Denis Netakhin <denis.netahin@yandex.ru>
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