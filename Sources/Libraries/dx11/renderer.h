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

//protected:
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

//protected:
	VertexBuffer meshVB;
	IndexBuffer meshIB;
	InputLayout layout;
	DrawIndexedPrimitive drawer;
};

//
//
//
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

//
//
//
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

// template<class MeshVertex, class Index>
// void Renderer<MeshVertex, Index>::draw()
// {
// 	layout.set();
// 	meshIB.set();
// 	meshVB.set<0>();
// 
// 	dx11::device.context->DrawIndexed(meshIB.count, 0, 0);
// }

}