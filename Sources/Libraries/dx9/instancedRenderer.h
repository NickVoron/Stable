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
#include "vertexDeclaration.h"
#include "dip.h"


namespace dx9
{

using namespace Geometry;

template<class InstanceVertex, class MeshVertex, class Index, class DIP>
class InstancedRenderer
{
public:
	typedef InstancedRenderer Renderer;

	void init(const VertexStream<MeshVertex>& meshVS, const IndexStream<Index>& meshIS, bool dynamic);
	bool initialized() const;
	
	VertexStreamBase<InstanceVertex>& lock(int count);
	VertexStreamBase<InstanceVertex>& instances() { return vs; }
	void unlock();

	void begin();
	void end();

	void draw(int count);

protected:
	VertexBuffer instanceVB;
	VertexBuffer meshVB;
	IndexBuffer meshIB;
	VertexDeclaration vd;
	DIP dip;
	VertexStreamBase<InstanceVertex> vs;
};





template<class InstanceVertex, class MeshVertex, class Index, class DIP>
void InstancedRenderer<InstanceVertex, MeshVertex, Index, DIP>::init(const VertexStream<MeshVertex>& meshVS, const IndexStream<Index>& meshIS, bool dynamic)
{
	instanceVB.setType(dynamic ? Buffer::DYNAMIC : Buffer::STATIC);
	meshVB.upload(meshVS);
	meshIB.upload(meshIS);
	vd.create<Loki::MakeTypelist<MeshVertex, InstanceVertex>>();
	dip.setVerticesCount(meshVB.getVerticesCount());
	dip.setIndicesCount(meshIB.getIndicesCount());
}

template<class InstanceVertex, class MeshVertex, class Index, class DIP>
bool InstancedRenderer<InstanceVertex, MeshVertex, Index, DIP>::initialized() const
{
	return dip.getPrimitivesCount() > 0;
}

template<class InstanceVertex, class MeshVertex, class Index, class DIP>
VertexStreamBase<InstanceVertex>& InstancedRenderer<InstanceVertex, MeshVertex, Index, DIP>::lock(int count)
{
	instanceVB.lock(0, count, vs);
	return vs;
}

template<class InstanceVertex, class MeshVertex, class Index, class DIP>
void InstancedRenderer<InstanceVertex, MeshVertex, Index, DIP>::unlock()
{
	instanceVB.unlock();
}

template<class InstanceVertex, class MeshVertex, class Index, class DIP>
void InstancedRenderer<InstanceVertex, MeshVertex, Index, DIP>::begin()
{
	vd.set();
	meshIB.set();
	meshVB.set<0>();
	instanceVB.set<1>();

	LPDIRECT3DDEVICE9 dev = dx9::device;
	dev->SetStreamSourceFreq( 1, ( D3DSTREAMSOURCE_INSTANCEDATA | 1 ));
}

template<class InstanceVertex, class MeshVertex, class Index, class DIP>
void InstancedRenderer<InstanceVertex, MeshVertex, Index, DIP>::end()
{
	LPDIRECT3DDEVICE9 dev = dx9::device;
	dev->SetStreamSourceFreq( 0, 1 );
	dev->SetStreamSourceFreq( 1, 1 );
}

template<class InstanceVertex, class MeshVertex, class Index, class DIP>
void InstancedRenderer<InstanceVertex, MeshVertex, Index, DIP>::draw(int count)
{
	LPDIRECT3DDEVICE9 dev = dx9::device;
	dev->SetStreamSourceFreq( 0, ( D3DSTREAMSOURCE_INDEXEDDATA | ( UINT )count ));

	dip.call();

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