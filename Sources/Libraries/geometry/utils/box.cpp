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
		// Lower tier (lower in y)
		{-.5f, -.5f, -.5f},
		{-.5f, -.5f,  .5f},
		{ .5f, -.5f,  .5f},
		{ .5f, -.5f, -.5f},

		// Upper tier
		{-.5f, .5f, -.5f},
		{-.5f, .5f,  .5f},
		{ .5f, .5f,  .5f},
		{ .5f, .5f, -.5f},
	};

	unsigned int indices = 0;
	unsigned int vertexIdx = 0;
	unsigned int indexIdx = 0;

	// i iterates over the faces, 2 triangles per face
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