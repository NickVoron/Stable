// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#if defined(USE_WINDOWS)

#include "geometry/library.include.h"
#include "dx9/library.include.h"
#include "gapi/library.include.h"

namespace dx9
{
	namespace im
	{
		template<class... VertexComponentsList>
		void SetVertexDeclaration()
		{
			VertexDeclaration decl;
			decl.create<Loki::MakeTypelist<Geometry::Vertex<VertexComponentsList...>>>();
			decl.set();
		}

		template<gapi::PrimitiveTopology pt, class... CL>
		void DrawPrimitiveUP( const Geometry::Vertex<CL...>* v, unsigned int vcount )
		{
			using namespace gapi;

			int pcount = PrimitiveCountEvaluator<pt>::eval(vcount);
			if(pcount > 0)
			{
				SetVertexDeclaration<CL...>();		
				device->DrawPrimitiveUP( Topology<DX9, pt>::topology, pcount, v, sizeof(Geometry::Vertex<CL...>));
			}	
		}

		template<gapi::PrimitiveTopology pt, class... CL, class IndexType>
		void DrawIndexedPrimitiveUP( const Geometry::Vertex<CL...>* v, unsigned int vcount, const IndexType* ind, unsigned int icount)
		{
			using namespace gapi;

			int pcount = PrimitiveCountEvaluator<pt>::eval(icount);
			if(pcount > 0)
			{
				D3DFORMAT ifmt = sizeof(IndexType) == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32;

				SetVertexDeclaration<CL...>();		
				device->DrawIndexedPrimitiveUP(Topology<DX9, pt>::topology, 0, vcount, pcount, ind, ifmt, v, sizeof(Geometry::Vertex<CL...>));
			}	
		}

		template<class... CL> void DrawTriangleStrip( const Geometry::Vertex<CL...>* v, unsigned int vcount ) { DrawPrimitiveUP<gapi::TRIANGLESTRIP>(v, vcount); }
		template<class... CL> void DrawTriangleList( const Geometry::Vertex<CL...>* v, unsigned int vcount ) { DrawPrimitiveUP<gapi::TRIANGLELIST>(v, vcount); }
		template<class... CL> void DrawLineStrip( const Geometry::Vertex<CL...>* v, unsigned int vcount ) { DrawPrimitiveUP<gapi::LINESTRIP>(v, vcount); }
		template<class... CL> void DrawLineList( const Geometry::Vertex<CL...>* v, unsigned int vcount ) { DrawPrimitiveUP<gapi::LINELIST>(v, vcount); }

		template<class... CL, class IndexType> void DrawTriangleStripIndexed(const Geometry::Vertex<CL...>* v, unsigned int vcount, const IndexType* ind, unsigned int icount) { DrawIndexedPrimitiveUP<gapi::TRIANGLESTRIP>(v, vcount, ind, icount); }
		template<class... CL, class IndexType> void DrawTriangleListIndexed(const Geometry::Vertex<CL...>* v, unsigned int vcount, const IndexType* ind, unsigned int icount) { DrawIndexedPrimitiveUP<gapi::TRIANGLELIST>(v, vcount, ind, icount); }
		template<class... CL, class IndexType> void DrawLineStripIndexed(const Geometry::Vertex<CL...>* v, unsigned int vcount, const IndexType* ind, unsigned int icount) { DrawIndexedPrimitiveUP<gapi::LINESTRIP>(v, vcount, ind, icount); }
		template<class... CL, class IndexType> void DrawLineListIndexed( const Geometry::Vertex<CL...>* v, unsigned int vcount, const IndexType* ind, unsigned int icount ) { DrawIndexedPrimitiveUP<gapi::LINELIST>(v, vcount, ind, icount); }

		template<class CL> void DrawTriangleStrip(const Geometry::VertexStream<CL>& vs) { DrawTriangleStrip(&vs[0], vs.size()); }
		template<class CL> void DrawTriangleList(const Geometry::VertexStream<CL>& vs) { DrawTriangleList(&vs[0], vs.size()); }
		template<class CL> void DrawLineStrip(const Geometry::VertexStream<CL>& vs) { DrawLineList(&vs[0], vs.size()); }
		template<class CL> void DrawLineList(const Geometry::VertexStream<CL>& vs) { DrawLineList(&vs[0], vs.size()); }

		template<class CL, class IndexType> void DrawTriangleStripIndexed(const Geometry::VertexStream<CL>& vs, const Geometry::IndexStream<IndexType>& is) { DrawTriangleStripIndexed(&vs[0], vs.size(), &is[0], is.size()); }
		template<class CL, class IndexType> void DrawTriangleListIndexed(const Geometry::VertexStream<CL>& vs, const Geometry::IndexStream<IndexType>& is) { DrawTriangleListIndexed(&vs[0], vs.size(), &is[0], is.size()); }
		template<class CL, class IndexType> void DrawLineStripIndexed(const Geometry::VertexStream<CL>& vs, const Geometry::IndexStream<IndexType>& is) { DrawLineListIndexed(&vs[0], vs.size(), &is[0], is.size()); }
		template<class CL, class IndexType> void DrawLineListIndexed(const Geometry::VertexStream<CL>& vs, const Geometry::IndexStream<IndexType>& is) { DrawLineListIndexed(&vs[0], vs.size(), &is[0], is.size()); }

		template<class CL, class IndexType> void DrawTriangleStripIndexed(const Geometry::Mesh<CL, IndexType>& mesh) { DrawTriangleStripIndexed(mesh.vertices(), mesh.indices()); }
		template<class CL, class IndexType> void DrawTriangleListIndexed(const Geometry::Mesh<CL, IndexType>& mesh) { DrawTriangleListIndexed(mesh.vertices(), mesh.indices()); }
		template<class CL, class IndexType> void DrawLineStripIndexed(const Geometry::Mesh<CL, IndexType>& mesh) { DrawLineListIndexed(mesh.vertices(), mesh.indices()); }
		template<class CL, class IndexType> void DrawLineListIndexed(const Geometry::Mesh<CL, IndexType>& mesh) { DrawLineListIndexed(mesh.vertices(), mesh.indices()); }


		void DrawFSQuad(int quadSize);
		void DrawFSQuad(int w, int h);
	}
}

#endif



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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