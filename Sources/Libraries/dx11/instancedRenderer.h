#pragma once

#include "geometry/library.include.h"

#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "inputLayout.h"
#include "drawCaller.h"

namespace dx11
{

template<class InstanceVertex, class MeshVertex, class Index>
class InstancedRenderer
{
public:
	typedef InstancedRenderer Renderer;

	void init(const Geometry::VertexStream<MeshVertex>& meshVS, const Geometry::IndexStream<Index>& meshIS);
	void init(const Geometry::Mesh<MeshVertex, Index>& mesh);
	bool initialized() const;
	
	void instances(const Geometry::VertexStream<InstanceVertex>& instancesVS);

	void setup(int cnt) { count = cnt; }

	void append(DrawableObjectsList& drawables)
	{
		meshVB.setup(0);
		instanceVB.setup(1);

		drawer.setup(meshIB.count(), count, 0, 0, 0);
		
		drawables.add(layout);
		drawables.add(meshIB);
		drawables.add(meshVB);
		drawables.add(instanceVB);
		drawables.add(drawer);
	}

protected:
	int count;
	VertexBuffer instanceVB;
	VertexBuffer meshVB;
	IndexBuffer meshIB;
	InputLayout layout;
	DrawIndexedInstancedPrimitive drawer;
};

//
//
//
template<class InstanceVertex, class MeshVertex, class Index>
void InstancedRenderer<InstanceVertex, MeshVertex, Index>::init(const Geometry::VertexStream<MeshVertex>& meshVS, const Geometry::IndexStream<Index>& meshIS)
{
	meshVB.upload(meshVS);
	meshIB.upload(meshIS);

	typedef Loki::MakeTypelist<MeshVertex, InstanceVertex> VertexList;
	layout.create<VertexList>();
}

template<class InstanceVertex, class MeshVertex, class Index>
bool InstancedRenderer<InstanceVertex, MeshVertex, Index>::initialized() const
{
	return meshIB.size() > 0;
}

template<class InstanceVertex, class MeshVertex, class Index>
void InstancedRenderer<InstanceVertex, MeshVertex, Index>::init(const Geometry::Mesh<MeshVertex, Index>& mesh)
{
	init(mesh.vertices(), mesh.indices());
}

template<class InstanceVertex, class MeshVertex, class Index>
void InstancedRenderer<InstanceVertex, MeshVertex, Index>::instances(const Geometry::VertexStream<InstanceVertex>& instancesVS)
{
	instanceVB.upload(instancesVS);
}

}