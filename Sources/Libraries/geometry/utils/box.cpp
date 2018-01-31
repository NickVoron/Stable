// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "bodies.h"
#include "boundings.h"
#include "meshOperations.h"

namespace Geometry {
namespace Generation {
namespace Private	{


void generateBox(BaseVertexStream& vs, BaseIndexStream& is, const Vector3& size)
{
	vs.resize(24);
	is.resize(36);

	static unsigned int cubeF[6][4] =
	{
		{ 0, 1, 5, 4 }, { 4, 5, 6, 7 }, { 7, 6, 2, 3 },
		{ 1, 0, 3, 2 }, { 1, 2, 6, 5 }, { 0, 4, 7, 3 }
	};

	static float cubeV[8][3] =
	{
		
		{-.5f, -.5f, -.5f},
		{-.5f, -.5f,  .5f},
		{ .5f, -.5f,  .5f},
		{ .5f, -.5f, -.5f},

		
		{-.5f, .5f, -.5f},
		{-.5f, .5f,  .5f},
		{ .5f, .5f,  .5f},
		{ .5f, .5f, -.5f},
	};

	unsigned int indices = 0;
	unsigned int vertexIdx = 0;
	unsigned int indexIdx = 0;

	
	for (int i=0; i<6; i++)
	{
		for (int j=0; j<4; j++)
		{
			vs[vertexIdx].pos3(cubeV[cubeF[i][j]][0] * size.x, cubeV[cubeF[i][j]][1] * size.y, cubeV[cubeF[i][j]][2] * size.z);
			vs[vertexIdx].uv2<0>(((j ^ (j >> 1)) & 1) ? 0.0f : 1.0f, (j & 2) ? 1.0f : 0.0f);
			
			vertexIdx++;
		}

		is[indexIdx++] = indices + 0;
		is[indexIdx++] = indices + 1;
		is[indexIdx++] = indices + 2;
		
		is[indexIdx++] = indices + 2;
		is[indexIdx++] = indices + 3;
		is[indexIdx++] = indices + 0;
		
	 	indices += 4;
	}

	generateNormals(vs, is);

}

void generateBox(BaseMesh& mesh, const Vector3& size)
{
	generateBox(mesh.vertices(), mesh.indices(), size);
}


}
}
}



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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