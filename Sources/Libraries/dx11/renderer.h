// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "geometry/library.include.h"

#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "inputLayout.h"
#include "drawCaller.h"

namespace dx11
{

template<class MeshVertex>
class VerticesRenderer
{
public:
	void init(const Geometry::VertexStream<MeshVertex>& meshVS);
	void init(const MeshVertex* vertices, int verticesCount);
	
	void append(DrawableObjectsList& drawables)
	{
		meshVB.setup(0);

		drawer.setup(meshVB.count(), 0);

		drawables.add(layout);
		drawables.add(meshVB);
		drawables.add(drawer);
	}


	VertexBuffer meshVB;
	InputLayout layout;
	DrawPrimitive drawer;
};

template<class MeshVertex, class Index>
class Renderer
{
public:
	void init(const Geometry::VertexStream<MeshVertex>& meshVS, const Geometry::IndexStream<Index>& meshIS);
	void init(const Geometry::Mesh<MeshVertex, Index>& mesh);
	void init(const MeshVertex* vertices, int verticesCount, const Index* indices, int indicesCount);
	
	void append(DrawableObjectsList& drawables)
	{
		meshVB.setup(0);
		
		drawer.setup(meshIB.count(), 0, 0);

		drawables.add(layout);
		drawables.add(meshIB);
		drawables.add(meshVB);
		drawables.add(drawer);
	}


	VertexBuffer meshVB;
	IndexBuffer meshIB;
	InputLayout layout;
	DrawIndexedPrimitive drawer;
};




template<class MeshVertex>
void VerticesRenderer<MeshVertex>::init(const Geometry::VertexStream<MeshVertex>& meshVS)
{
	init(&meshVS[0], (int)meshVS.size());
}

template<class MeshVertex>
void VerticesRenderer<MeshVertex>::init(const MeshVertex* vertices, int verticesCount)
{
	meshVB.upload(vertices, verticesCount);
	layout.create<Loki::MakeTypelist<MeshVertex>>();
}




template<class MeshVertex, class Index>
void Renderer<MeshVertex, Index>::init(const Geometry::VertexStream<MeshVertex>& meshVS, const Geometry::IndexStream<Index>& meshIS)
{
	init(&meshVS[0], (int)meshVS.size(), &meshIS[0], (int) meshIS.size());
}

template<class MeshVertex, class Index>
void Renderer<MeshVertex, Index>::init(const MeshVertex* vertices, int verticesCount, const Index* indices, int indicesCount)
{
	meshVB.upload(vertices, verticesCount);
	meshIB.upload(indices, indicesCount);
	layout.create<Loki::MakeTypelist<MeshVertex>>();
}

template<class MeshVertex, class Index>
void Renderer<MeshVertex, Index>::init(const Geometry::Mesh<MeshVertex, Index>& mesh)
{
	init(mesh.vertices(), mesh.indices());
}











}



// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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