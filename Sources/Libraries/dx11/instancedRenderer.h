// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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