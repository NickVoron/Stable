// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#ifdef USE_WINDOWS

#include "dx9/library.include.h"

namespace dx9 
{
namespace Utils 
{
	template<class... VertexComponentsList>
	class ScreenQuad
	{
	public:

		typedef Geometry::Vertex< VertexComponentsList... > ScreenQuadVertex;
		typedef unsigned int ScreenQuadIndex;
		typedef Geometry::Mesh< ScreenQuadVertex, ScreenQuadIndex > ScreenQuadMesh;

		void init();
		void init(unsigned int width, unsigned int height);
		void init(unsigned int width, unsigned int height, bool absoluteUV);
		void draw();
		void upload();

		ScreenQuadMesh mesh;

	private:
		
		template<bool enabled>	void generateUV(unsigned int width, unsigned int height, bool absoluteUV);
		template<> void generateUV<false>(unsigned int width, unsigned int height, bool absoluteUV){};
		template<> void generateUV<true>(unsigned int width, unsigned int height, bool absoluteUV)
		{
			ScreenQuadMesh::VertexStreamType& vs = mesh.vertices();
			
			float w = absoluteUV ? (float)width : 1.0f;
			float h = absoluteUV ? (float)height : 1.0f;

			vs[0].uv2<0>() = UV2<0>(0.0f, 0.0f);
			vs[1].uv2<0>() = UV2<0>(1.0f * w, 0.0f);
			vs[2].uv2<0>() = UV2<0>(0.0f, 1.0f * h);
			vs[3].uv2<0>() = UV2<0>(1.0f * w, 1.0f * h);
		}

		dx9::DynamicMesh drawer;
   	};


	template<class... VertexComponentsList>
	void ScreenQuad<VertexComponentsList...>::init()
	{
		unsigned int width, height;

		init(width, height);
	}

	template<class... VertexComponentsList>
	void ScreenQuad<VertexComponentsList...>::init(unsigned int width, unsigned int height)
	{
		init(width, height, false);	
	}

	template<class... VertexComponentsList>
	void ScreenQuad<VertexComponentsList...>::init(unsigned int width, unsigned int height, bool absoluteUV)
	{
		ScreenQuadMesh::VertexStreamType& vs = mesh.vertices();
		ScreenQuadMesh::IndexStreamType& is = mesh.indices();

		vs.resize(4);
		is.resize(6);

		vs[0].posT(0, 0, 1, 1);
		vs[1].posT((float)width, 0, 1, 1);
		vs[2].posT(0, (float)height, 1, 1);
		vs[3].posT((float)width, (float)height, 1, 1);

		generateUV< IsVertexHas<ScreenQuadVertex, UV2<0> >::value >(width, height, absoluteUV);

		is[0] = 0;
		is[1] = 1;
		is[2] = 2;
		is[3] = 2;
		is[4] = 1;
		is[5] = 3;
	}

	template<class... VertexComponentsList>
	void ScreenQuad<VertexComponentsList...>::draw()
	{



		drawer.draw();



	}

	template<class... VertexComponentsList>
	void ScreenQuad<VertexComponentsList...>::upload()
	{
		drawer.uploadMesh(mesh);
	}
	
}
}

#endif



// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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