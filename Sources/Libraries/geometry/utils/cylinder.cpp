#include "bodies.h"
#include "meshOperations.h"

namespace Geometry {
namespace Generation {
namespace Private	{


	namespace
	{
		static inline void sincosf(float angle, float *psin, float *pcos)
		{
			*psin = sinf(angle);
			*pcos = cosf(angle);
		}

		static void	MakeCylinder( BaseVertexStream& vertices, BaseIndexStream& indices, float radius1, float radius2, float fLength, unsigned int slices, unsigned int stacks)
		{
			unsigned int vrtCount = 2 + (stacks + 3) * slices;
			unsigned int idxCount = (stacks + 1) * slices * 2 * 3;

			vertices.resize(vrtCount);
			indices.resize(idxCount);

			static const int CACHE_SIZE = 240;
			
			// Sin/Cos caches
			float sinI[CACHE_SIZE];
			float cosI[CACHE_SIZE];

			for(unsigned int i = 0; i < slices; i++) 
				sincosf(nm::TwoPI * i / slices, sinI + i, cosI + i);


			// Compute side normal angle
			float fDeltaRadius = radius2 - radius1;
			float fSideLength = sqrtf(fDeltaRadius * fDeltaRadius + fLength * fLength);

			// Generate vertices
						
			// Base cap (slices + 1)
			//fZ = fLength * -0.5f;
			float fZ = 0.0f;
			float radius = radius1;
			int vtx = 0;

			vertices[vtx++].pos3(0.0f, fZ, 0.0f);
			

			for(unsigned int i = 0; i < slices; i++)
			{
				vertices[vtx++].pos3(radius * -sinI[i], fZ, radius * cosI[i]);
			}

			// Stacks ((stacks + 1)*slices)
			for(unsigned int j = 0; j <= stacks; j++)
			{
				float f = (float) j / (float) stacks;

				fZ = fLength * (f);
				radius = radius1 + f * fDeltaRadius;

				for(unsigned int i = 0; i < slices; i++)
				{
					vertices[vtx++].pos3(radius * -sinI[i], fZ, radius * cosI[i]);
				}
			}

			// Top cap (slices + 1)
			//fZ = fLength * 0.5f;
			fZ = fLength;
			radius = radius2;

			for(unsigned int i = 0; i < slices; i++)
			{
				vertices[vtx++].pos3(radius * -sinI[i], fZ, radius * cosI[i]);
			}

			vertices[vtx++].pos3(0.0f, fZ, 0.0f);

			// Generate indices
			unsigned int indexIdx = 0;
			
			// Z+ pole (slices)
			unsigned int rowA = 0;
			unsigned int rowB = 1;

			for(unsigned int i = 0; i < slices - 1; i++)
			{
				indices[indexIdx++] = rowB + i + 1;
				indices[indexIdx++] = rowA;
				indices[indexIdx++] = rowB + i;
			}

			indices[indexIdx++] = rowB;
			indices[indexIdx++] = rowB + slices - 1;
			indices[indexIdx++] = rowA;

			// Interior stacks (stacks * slices * 2)
			for(unsigned int j = 0; j < stacks; j++)
			{
				rowA = 1 + (j + 1) * slices;
				rowB = rowA + slices;

				unsigned int end = slices - 1;
				for(unsigned int i = 0; i < end; i++)
				{
					indices[indexIdx++] = (rowA + i);
					indices[indexIdx++] = (rowB + i);
					indices[indexIdx++] = (rowA + i + 1);

					indices[indexIdx++] = (rowA + i + 1);
					indices[indexIdx++] = (rowB + i);
					indices[indexIdx++] = (rowB + i + 1);
				}

				indices[indexIdx++] = (rowA + end);
				indices[indexIdx++] = (rowB + end);
				indices[indexIdx++] = (rowA);

				indices[indexIdx++] = (rowA);
				indices[indexIdx++] = (rowB + end);
				indices[indexIdx++] = (rowB);
			}

			// Z- pole (slices)
			rowA = 1 + (stacks + 2) * slices;
			rowB = rowA + slices;

			unsigned int end = slices - 1;
			for(unsigned int i = 0; i < end; i++)
			{
				indices[indexIdx++] = (rowA + i);
				indices[indexIdx++] = (rowB);
				indices[indexIdx++] = (rowA + i + 1);
			}

			indices[indexIdx++] = (rowA + end);
			indices[indexIdx++] = (rowB);
			indices[indexIdx++] = (rowA);
		}
	}



void generateCylinder(BaseVertexStream& vs, BaseIndexStream& is, float radius, float height, unsigned int slices, unsigned int stacks)
{
	MakeCylinder(vs, is, radius, radius, height, slices, stacks);
	generateNormals(vs, is);

	BaseVertexStream& vrtStream = vs;

	for(std::size_t v = 0; v < vrtStream.size(); v++)
	{

		if(vrtStream[v].normal().y != 0.0f)	
		{
			vrtStream[v].uv2<0>(vrtStream[v].normal().y * vrtStream[v].pos3().x, vrtStream[v].pos3().z);
		}
		else
		{
			if(vrtStream[v].pos3().x>0.0f)
				if(vrtStream[v].pos3().z>=0.0f)	vrtStream[v].uv2<0>(asinf(vrtStream[v].pos3().x), vrtStream[v].pos3().y+0.5f);
				else	vrtStream[v].uv2<0>(3.1415926535897932f*0.5f+acosf(vrtStream[v].pos3().x), vrtStream[v].pos3().y+0.5f);
			else
				if(vrtStream[v].pos3().z>=0.0f)	vrtStream[v].uv2<0>(3.1415926535897932f*1.5f + acosf(-vrtStream[v].pos3().x), vrtStream[v].pos3().y+0.5f);
				else	vrtStream[v].uv2<0>(3.1415926535897932f+asinf(-vrtStream[v].pos3().x), vrtStream[v].pos3().y+0.5f);
				vrtStream[v].uv2<0>().u *= -1.0f;
		}
	}
}

void generateCylinder(BaseMesh& mesh, float radius, float height, unsigned int slices, unsigned int stacks)
{
	generateCylinder(mesh.vertices(), mesh.indices(), radius, height, slices, stacks);
}

void generateCone(BaseVertexStream& vs, BaseIndexStream& is, float radiusTop, float radiusBottom, float height, unsigned int slices, unsigned int stacks)
{
	MakeCylinder(vs, is, radiusBottom, radiusTop + 0.00001f, height, slices, stacks);
	generateNormals(vs, is);
}

void generateCone(BaseMesh& mesh, float radiusTop, float radiusBottom, float height, unsigned int slices, unsigned int stacks)
{
	generateCone(mesh.vertices(), mesh.indices(), radiusTop, radiusBottom, height, slices, stacks);
}

}
}
}