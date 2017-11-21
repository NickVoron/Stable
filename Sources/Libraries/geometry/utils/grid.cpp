#include "bodies.h"
#include "meshOperations.h"

namespace Geometry {
namespace Generation {
namespace Private	{

void generateGridIndices(BaseIndexStream& indices, const nm::index2& vertexCount)
{
	indices.resize( vertexCount.Square() * 2 * 3 );
	// indices:
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

	//// создаим блоцк 64 на 64 квадика или 65х65 вершинок
	// vertex buffer structure:
	vertices.resize(vertexCount.Square());

	Vector2 maxIndex = nm::floatize( vertexCount - nm::index2(1, 1) );
	Vector2 invMaxIdx(1.0f / maxIndex.x, 1.0f / maxIndex.y);
	nm::index2 vertexId;
	Vector2 halfSize = nm::floatize(vertexCount) / 2.0f;
	// vertices:
	for ( vertexId.y = 0; vertexId.y < vertexCount.y; ++vertexId.y )
	{
		for ( vertexId.x = 0; vertexId.x < vertexCount.x; ++vertexId.x )
		{
			// relative index:
			Vector2 curIndex = nm::floatize( vertexId );

			// vertex:
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

	//// создаим блоцк 64 на 64 квадика или 65х65 вершинок
	// vertex buffer structure:
	vertices.resize(vertexCount.Square());

	Vector2 maxIndex = nm::floatize( vertexCount - nm::index2( 1, 1 ) );
	Vector2 invMaxIdx(1.0f / maxIndex.x, 1.0f / maxIndex.y);
	nm::index2 vertexId;
	Vector2 halfSize = nm::floatize(vertexCount) / 2.0f;
	// vertices:
	for ( vertexId.y = 0; vertexId.y < vertexCount.y; ++vertexId.y )
	{
		for ( vertexId.x = 0; vertexId.x < vertexCount.x; ++vertexId.x )
		{
			// relative index:
			Vector2 curIndex = nm::floatize( vertexId );

			// vertex:
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


} // -namespace
}
}