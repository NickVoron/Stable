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

//
//
//

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