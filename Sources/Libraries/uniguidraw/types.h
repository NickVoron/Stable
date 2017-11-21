#pragma once

#include "geometry/library.include.h"
#include "unigui/library.include.h"
#include "drawers.h"


namespace unigui {
namespace draw {
	
		using namespace Geometry;

		typedef Vertex< UV4<0>, DIFFUSE> RInstVtx;
		typedef Vertex< UV4<0>, UV4<1>, DIFFUSE > TRInstVtx;
		typedef Vertex< POS2 > MeshVtx;

		typedef Vertex< UV4<0>, UV4<1>, UV4<2>, DIFFUSE > LineInstVtx;
		typedef Vertex< POS2 > LineVtx;

		enum PrimitiveRenderType
		{
			PRT_LINE,
			PRT_SOLID_RECT,
			PRT_TEXTURED_RECT,
			PRT_FONT,

			PRIMITIVE_TYPES
		};

		namespace Private
		{
			template<class VertexType, class IndexType>
			void initQuadPrimitive(VertexStream<VertexType>& vs, IndexStream<IndexType>& is)
			{
				vs.resize(4);
				vs[0].pos2(0.0f, 0.0f);
				vs[1].pos2(1.0f, 0.0f);
				vs[2].pos2(0.0f, 1.0f);
				vs[3].pos2(1.0f, 1.0f);

				is.resize(6);
				is[0] = 0;
				is[1] = 1;
				is[2] = 2;
				is[3] = 2;
				is[4] = 3;
				is[5] = 1;
			}
		}

		//
		//
		//
		struct RendererUsingInterface
		{
			virtual void add(const math::Rect<float>& r, const Color& color) {}
			virtual void add(const math::Rect<float>& r, const math::Rect<float>& tr, const Color& color) {}
			virtual void add(const math::Point2<float>& p0, const math::Point2<float>& p1, const Color& color) {}
			virtual void add(const math::Rect<float>& r, const math::Point2<float>& p0, const math::Point2<float>& p1, float thickness, const Color& color) {}
		};

}
}