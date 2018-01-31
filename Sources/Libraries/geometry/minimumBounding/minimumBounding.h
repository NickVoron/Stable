// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "../mesh/vertexStream.h"
#include "intersections/library.include.h"

namespace Geometry
{
	void computeBestFitOBB(unsigned int vcount, const float *points, unsigned int pstride, float *sides, float *matrix);
	void computeBestFitOBB(unsigned int vcount, const float *points, unsigned int pstride, float *sides, float *pos, float *quat);
	void computeBestFitABB(unsigned int vcount, const float *points, unsigned int pstride, float *sides, float *pos);

	template<class VertexCL>
	void computeOptimalOBB(const Geometry::VertexStream< Geometry::Vertex<VertexCL> >& vertices, intersections::OBB& obb)
	{
		Quaternion quat;
		float sides[3];

		computeBestFitOBB(vertices.size(), (float*)&vertices[0].pos3(), sizeof(Geometry::Vertex<VertexCL>), sides, (float*)&obb.center, (float*)&quat);

		obb.extent[0] = sides[0] * 0.5;
		obb.extent[1] = sides[1] * 0.5;
		obb.extent[2] = sides[2] * 0.5;

		obb.axis[0] = quat.Transform(Vector3::xAxis);
		obb.axis[1] = quat.Transform(Vector3::yAxis);
		obb.axis[2] = quat.Transform(Vector3::zAxis);
	}

	
	
	
	
	
	
	

	

	
	
	
	
	
	
	
	

	

	
	

	
	
	
	

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