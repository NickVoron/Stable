#include "immediateDraw.h"

namespace dx11
{
	namespace im
	{
		std::unique_ptr<VertexBuffer> vb;
		std::unique_ptr<IndexBuffer> ib;

		//
		//
		//
		void DrawFSQuad(const Viewports::Viewport& viewport)
		{
			typedef Geometry::Vertex<Geometry::POS3, Geometry::UV2<0>> ScreenQuadVertex;
			ScreenQuadVertex quad[4];

			quad[0].pos3(-1.0f, -1.0f, 0.5f);
			quad[0].uv2<0>(0.0f, 1.0f);

			quad[1].pos3(1.0, -1.0f, 0.5f);
			quad[1].uv2<0>(1.0f, 1.0f);

			quad[2].pos3(-1.0f, 1.0, 0.5f);
			quad[2].uv2<0>(0.0f, 0.0f);

			quad[3].pos3(1.0, 1.0, 0.5f);
			quad[3].uv2<0>(1.0f, 0.0f);

			DrawTriangleStrip(viewport, &quad[0], 4);
		}

		void DrawFSQuad(const Viewports::Viewport& viewport, int w, int h)
		{
			DrawFSQuad(viewport);
		}
	}
}
