#include "dx9.h"

namespace dx9 
{
	namespace im 
	{

		void DrawFSQuad(int quadSize)
		{
			DrawFSQuad(quadSize, quadSize);
		}

		void DrawFSQuad(int w, int h)
		{
			float fw = (float)w - 0.5f;
			float fh = (float)h - 0.5f;

			typedef Geometry::Vertex<Geometry::POST, Geometry::UV2<0>> ScreenQuadVertex;
			ScreenQuadVertex quad[4];

			quad[0].posT(-0.5f, -0.5f, 0.0f, 1.0f);
			quad[0].uv2<0>(0.0f, 0.0f);

			quad[1].posT(fw, -0.5f, 0.0f, 1.0f);
			quad[1].uv2<0>(1.0f, 0.0f);

			quad[2].posT(-0.5f, fh, 0.0f, 1.0f);
			quad[2].uv2<0>(0.0f, 1.0f);

			quad[3].posT(fw, fh, 0.0f, 1.0f);
			quad[3].uv2<0>(1.0f, 1.0f);

			DrawTriangleStrip(&quad[0], 4);
		}
	}
}