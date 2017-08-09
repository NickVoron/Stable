// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "bodies.h"
#include "meshOperations.h"

namespace Geometry {
namespace Generation {
namespace Private	{

void generateGridIndices(BaseIndexStream& indices, const nm::index2& vertexCount)
{
	indices.resize( vertexCount.Square() * 2 * 3 );
	
	nm::index2 quadId;
	int currentIndex = 0;
	for ( quadId.y = 0; quadId.y < vertexCount.y; ++quadId.y )
	{
		for ( quadId.x = 0; quadId.x < vertexCount.x; ++quadId.x )
		{
			const int vertShift = vertexCount.x+1;
			const int basePoint = quadId.x + quadId.y * vertShift;

			indices[ currentIndex++ ] = basePoint;
			indices[ currentIndex++ ] = basePoint + vertShift;
			indices[ currentIndex++ ] = basePoint + 1;

			indices[ currentIndex++ ] = basePoint + 1;
			indices[ currentIndex++ ] = basePoint + vertShift;
			indices[ currentIndex++ ] = basePoint + vertShift + 1;
		}
	}

	indices.updateMaxValue();
}

void generateGrid(BaseVertexStream& vertices, BaseIndexStream& indices, const nm::index2& vc, const Vector2& gridStep )
{
	nm::index2 vertexCount = vc + nm::index2(1, 1);
	generateGridIndices(indices, vc);

	
	
	vertices.resize(vertexCount.Square());

	Vector2 maxIndex = nm::floatize( vertexCount - nm::index2(1, 1) );
	Vector2 invMaxIdx(1.0f / maxIndex.x, 1.0f / maxIndex.y);
	nm::index2 vertexId;
	Vector2 halfSize = nm::floatize(vertexCount) / 2.0f;
	
	for ( vertexId.y = 0; vertexId.y < vertexCount.y; ++vertexId.y )
	{
		for ( vertexId.x = 0; vertexId.x < vertexCount.x; ++vertexId.x )
		{
			
			Vector2 curIndex = nm::floatize( vertexId );

			
			BaseVertexStream::Vertex& vertex = vertices[ vertexId.x + vertexId.y * vertexCount.x ];
			vertex.pos3().x = (curIndex.x - halfSize.x) * gridStep.x;
			vertex.pos3().z = (curIndex.y - halfSize.y) * gridStep.y;
			vertex.pos3().y = 0.0f;
			vertex.uv2<0>(vertex.pos3().x, vertex.pos3().z);
		}
	}

	generateNormals(vertices, indices);
}

void generateGrid2D(BaseVertex2DStream& vertices, BaseIndexStream& indices, const nm::index2& vertexCount, const Vector2& gridStep )
{
	generateGridIndices(indices, vertexCount);

	
	
	vertices.resize(vertexCount.Square());

	Vector2 maxIndex = nm::floatize( vertexCount - nm::index2( 1, 1 ) );
	Vector2 invMaxIdx(1.0f / maxIndex.x, 1.0f / maxIndex.y);
	nm::index2 vertexId;
	Vector2 halfSize = nm::floatize(vertexCount) / 2.0f;
	
	for ( vertexId.y = 0; vertexId.y < vertexCount.y; ++vertexId.y )
	{
		for ( vertexId.x = 0; vertexId.x < vertexCount.x; ++vertexId.x )
		{
			
			Vector2 curIndex = nm::floatize( vertexId );

			
			BaseVertex2DStream::Vertex& vertex = vertices[ vertexId.x + vertexId.y * vertexCount.x ];
			vertex.pos2().x = (curIndex.x - halfSize.x) * gridStep.x;
			vertex.pos2().y = (curIndex.y - halfSize.y) * gridStep.y;
		}
	}
}

void generateGrid(BaseMesh& mesh, const nm::index2& vertexCount, const Vector2& gridStep )
{
	generateGrid(mesh.vertices(), mesh.indices(), vertexCount, gridStep);
}

void generateGrid2D(BaseMesh2D& mesh, const nm::index2& vertexCount, const Vector2& gridStep )
{
	generateGrid2D(mesh.vertices(), mesh.indices(), vertexCount, gridStep);
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