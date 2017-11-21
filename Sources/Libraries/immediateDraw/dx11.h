#pragma once

#include "dx11/library.include.h"
#include "gapi/library.include.h"

namespace dx11
{
	namespace im
	{
		template<class... VertexComponentsList>
		InputLayout& GetInputLayout()
		{
			typedef Loki::MakeTypelist<Geometry::Vertex<VertexComponentsList...>> VertexList;

			static InputLayout layout;
			if(layout.empty())
			{
				layout.create<VertexList>();
			}

			return layout;
		}

		extern std::unique_ptr<VertexBuffer> vb;
		extern std::unique_ptr<IndexBuffer> ib;

		template<gapi::PrimitiveTopology pt, class... CL>
		void DrawPrimitiveUP(const Viewports::Viewport& viewport, const Geometry::Vertex<CL...>* v, unsigned int vcount)
		{
			DrawableObjectsList drawables;

			if (!vb)
			{
				vb = std::make_unique<VertexBuffer>();
			}

			vb->upload(v, vcount);
			vb->setup(0);
										   		
			dx11::DrawPrimitive drawer; drawer.setup(vcount, 0);
			dx11::SetPrimitiveTopology<pt> top;

			drawables.add(*vb);
			drawables.add(GetInputLayout<CL...>());
			drawables.add(top);
			drawables.add(drawer);

			dx11::execute(viewport, drawables);
		}

		template<gapi::PrimitiveTopology pt, class... CL>
		void DrawPrimitiveUP(const Viewports::Viewport& viewport, const Geometry::VertexStream<CL...>& vs)
		{
			DrawPrimitiveUP<pt>(viewport, &vs[0], vs.getSize());
		}
	   
		template<gapi::PrimitiveTopology pt, class... CL, class IndexType>
		void DrawIndexedPrimitiveUP(const Viewports::Viewport& viewport, const Geometry::Vertex<CL...>* v, unsigned int vcount, const IndexType* ind, unsigned int icount)
		{
			DrawableObjectsList drawables;

			if (!vb)
			{
				vb = std::make_unique<VertexBuffer>();
			}

			vb->upload(v, vcount);
			vb->setup(0);

			if (!ib)
			{
				ib = std::make_unique<IndexBuffer>();
			}

			ib->upload(ind, icount);

			dx11::DrawIndexedPrimitive drawer; drawer.setup(icount, 0, 0);
			dx11::SetPrimitiveTopology<pt> top;

			drawables.add(*vb);
			drawables.add(*ib);
			drawables.add(GetInputLayout<CL...>());
			drawables.add(top);
			drawables.add(drawer);

			dx11::execute(viewport, drawables);
		}

		template<gapi::PrimitiveTopology pt, class... CL, class IndexType>	void DrawIndexedPrimitiveUP(const Viewports::Viewport& viewport, const Geometry::VertexStream<CL...>& vs, const Geometry::IndexStream<IndexType>& is)	{ DrawIndexedPrimitiveUP<pt>(viewport, &vs[0], vs.size(), &is[0], is.size()); }
		template<gapi::PrimitiveTopology pt, class CL, class IndexType>	void DrawIndexedPrimitiveUP(const Viewports::Viewport& viewport, const Geometry::Mesh<CL, IndexType>& mesh)													{ DrawIndexedPrimitiveUP<pt>(viewport, mesh.vertices(), mesh.indices()); }

		template<class... CL>					void DrawTriangleStrip(const Viewports::Viewport& viewport, const Geometry::Vertex<CL...>* v, unsigned int vcount)																	{ DrawPrimitiveUP<gapi::TRIANGLESTRIP>(viewport, v, vcount); }
		template<class... CL>					void DrawTriangleList(const Viewports::Viewport& viewport, const Geometry::Vertex<CL...>* v, unsigned int vcount)																	{ DrawPrimitiveUP<gapi::TRIANGLELIST>(viewport, v, vcount); }
		template<class... CL>					void DrawLineStrip(const Viewports::Viewport& viewport, const Geometry::Vertex<CL...>* v, unsigned int vcount)																		{ DrawPrimitiveUP<gapi::LINESTRIP>(viewport, v, vcount); }
		template<class... CL>					void DrawLineList(const Viewports::Viewport& viewport, const Geometry::Vertex<CL...>* v, unsigned int vcount)																		{ DrawPrimitiveUP<gapi::LINELIST>(viewport, v, vcount); }

		template<class... CL, class IndexType> void DrawTriangleStripIndexed(const Viewports::Viewport& viewport, const Geometry::Vertex<CL...>* v, unsigned int vcount, const IndexType* ind, unsigned int icount)				{ DrawIndexedPrimitiveUP<gapi::TRIANGLESTRIP>(viewport, v, vcount, ind, icount); }
		template<class... CL, class IndexType> void DrawTriangleListIndexed(const Viewports::Viewport& viewport, const Geometry::Vertex<CL...>* v, unsigned int vcount, const IndexType* ind, unsigned int icount)				{ DrawIndexedPrimitiveUP<gapi::TRIANGLELIST>(viewport, v, vcount, ind, icount); }
		template<class... CL, class IndexType> void DrawLineStripIndexed(const Viewports::Viewport& viewport, const Geometry::Vertex<CL...>* v, unsigned int vcount, const IndexType* ind, unsigned int icount)					{ DrawIndexedPrimitiveUP<gapi::LINESTRIP>(viewport, v, vcount, ind, icount); }
		template<class... CL, class IndexType> void DrawLineListIndexed(const Viewports::Viewport& viewport, const Geometry::Vertex<CL...>* v, unsigned int vcount, const IndexType* ind, unsigned int icount)					{ DrawIndexedPrimitiveUP<gapi::LINELIST>(viewport, v, vcount, ind, icount); }

		template<class CL>					void DrawTriangleStrip(const Viewports::Viewport& viewport, const Geometry::VertexStream<CL>& vs)																				{ DrawTriangleStrip(viewport, &vs[0], vs.size()); }
		template<class CL>					void DrawTriangleList(const Viewports::Viewport& viewport, const Geometry::VertexStream<CL>& vs)																				{ DrawTriangleList(viewport, &vs[0], vs.size()); }
		template<class CL>					void DrawLineStrip(const Viewports::Viewport& viewport, const Geometry::VertexStream<CL>& vs)																					{ DrawLineList(viewport, &vs[0], vs.size()); }
		template<class CL>					void DrawLineList(const Viewports::Viewport& viewport, const Geometry::VertexStream<CL>& vs)																					{ DrawLineList(viewport, &vs[0], vs.size()); }

		template<class CL, class IndexType> void DrawTriangleStripIndexed(const Viewports::Viewport& viewport, const Geometry::VertexStream<CL>& vs, const Geometry::IndexStream<IndexType>& is)							{ DrawTriangleStripIndexed(viewport, &vs[0], vs.size(), &is[0], is.size()); }
		template<class CL, class IndexType> void DrawTriangleListIndexed(const Viewports::Viewport& viewport, const Geometry::VertexStream<CL>& vs, const Geometry::IndexStream<IndexType>& is)								{ DrawTriangleListIndexed(viewport, &vs[0], vs.size(), &is[0], is.size()); }
		template<class CL, class IndexType> void DrawLineStripIndexed(const Viewports::Viewport& viewport, const Geometry::VertexStream<CL>& vs, const Geometry::IndexStream<IndexType>& is)								{ DrawLineListIndexed(viewport, &vs[0], vs.size(), &is[0], is.size()); }
		template<class CL, class IndexType> void DrawLineListIndexed(const Viewports::Viewport& viewport, const Geometry::VertexStream<CL>& vs, const Geometry::IndexStream<IndexType>& is)									{ DrawLineListIndexed(viewport, &vs[0], vs.size(), &is[0], is.size()); }

		template<class CL, class IndexType> void DrawTriangleStripIndexed(const Viewports::Viewport& viewport, const Geometry::Mesh<CL, IndexType>& mesh)																	{ DrawTriangleStripIndexed(viewport, mesh.vertices(), mesh.indices()); }
		template<class CL, class IndexType> void DrawTriangleListIndexed(const Viewports::Viewport& viewport, const Geometry::Mesh<CL, IndexType>& mesh)																	{ DrawTriangleListIndexed(viewport, mesh.vertices(), mesh.indices()); }
		template<class CL, class IndexType> void DrawLineStripIndexed(const Viewports::Viewport& viewport, const Geometry::Mesh<CL, IndexType>& mesh)																		{ DrawLineListIndexed(viewport, mesh.vertices(), mesh.indices()); }
		template<class CL, class IndexType> void DrawLineListIndexed(const Viewports::Viewport& viewport, const Geometry::Mesh<CL, IndexType>& mesh)																		{ DrawLineListIndexed(viewport, mesh.vertices(), mesh.indices()); }

		void DrawFSQuad(const Viewports::Viewport& viewport);
		void DrawFSQuad(const Viewports::Viewport& viewport, int w, int h);

	}

}