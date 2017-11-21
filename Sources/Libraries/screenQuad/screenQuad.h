#pragma once


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
//		Globals::GetBackBufferSize(width, height);
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
// 		effect->Begin(0,0);
// 		effect->BeginPass(0);

		drawer.draw();

// 		effect->EndPass();
// 		effect->End();
	}

	template<class... VertexComponentsList>
	void ScreenQuad<VertexComponentsList...>::upload()
	{
		drawer.uploadMesh(mesh);
	}
	
}
}