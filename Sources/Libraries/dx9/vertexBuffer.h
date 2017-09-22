// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "geometry/mesh/vertexStream.h"
#include "buffer.h"
#include "device.h"

namespace dx9
{


class VertexBuffer : public BufferBase<VertexBuffer>
{
public:
	VertexBuffer();
	~VertexBuffer();

	template<class VertexType>
	void upload(const Geometry::VertexStream<VertexType>& vs);

	template<int streamId> 
	void set() const;

	unsigned int getVerticesCount() const{return verticesCount;}

	LPDIRECT3DVERTEXBUFFER9 getVB() const {return vb;}
	void clear();

	void recreate(unsigned int sz);

	template<class VertexType>
	void lock(int offsetInVertices, int verticesCount, Geometry::VertexStreamBase<VertexType>& vsb);
	template<class VertexType>
	void updateVerticesCountFrom(Geometry::VertexStreamBase<VertexType>& vsb);

	void unlock();

	virtual void updateFromCache();

private:
	void release() {SAFE_RELEASE(vb);}

	LPDIRECT3DVERTEXBUFFER9 vb;

	unsigned int sizeInBytes;	
	unsigned int sizeOfVertex;
	unsigned int verticesCount;
};





inline VertexBuffer::VertexBuffer()
:vb(0), sizeInBytes(0), sizeOfVertex(0), verticesCount(0)
{}

inline VertexBuffer::~VertexBuffer()
{ 
	clear(); 
}

template<class VertexType>
void VertexBuffer::upload(const Geometry::VertexStream<VertexType>& vs)
{
 	verticesCount = (unsigned int)vs.size();
 	if(verticesCount == 0) return;
 
 	VertexStreamBase<VertexType> vsb;
 	lock(0, verticesCount, vsb);
 	mem::memcpy( &vsb[0], &vs[0], verticesCount * sizeof(VertexType) );
 	createCache(&vs[0], verticesCount * sizeof(VertexType));
 	unlock();
}

inline void VertexBuffer::updateFromCache()
{
	if(cacheSize > 0)
		recreate(cacheSize);

	if(vb && cacheData && cacheSize)
	{
		void* dataPtr;
		DWORD flags = (type == DYNAMIC) ? D3DLOCK_DISCARD : 0;

		vb->Lock(0, cacheSize, &dataPtr, flags);
		mem::memcpy( dataPtr, cacheData, cacheSize );
		vb->Unlock();
	}
}

template<int streamId>
void VertexBuffer::set() const 
{ 
	static LPDIRECT3DVERTEXBUFFER9 currentVB = 0;
	
	{
		device->SetStreamSource( streamId, vb, 0, sizeOfVertex );
		currentVB = vb;
		incrementCounter();
	}
}

template<class VertexType>
void VertexBuffer::lock(int offsetInVertices, int verticesCount, Geometry::VertexStreamBase<VertexType>& vsb)
{
	sizeOfVertex = sizeof(VertexType);
	unsigned int estimatedSize = verticesCount * sizeof(VertexType);
	unsigned int offset = offsetInVertices * sizeof(VertexType);

	if(sizeInBytes < estimatedSize)
	{
		recreate(estimatedSize);	
	}

	if(vb)
	{
		void* dataPtr;
		DWORD flags = ((type == DYNAMIC) ? D3DLOCK_DISCARD : 0);

		vb->Lock(offset, estimatedSize, &dataPtr, flags);
		vsb = Geometry::VertexStreamBase<VertexType>(dataPtr, verticesCount);
	}
	
}

inline void VertexBuffer::unlock()
{
	if(vb)
	{
		vb->Unlock();
	}
}

template<class VertexType>
void VertexBuffer::updateVerticesCountFrom(Geometry::VertexStreamBase<VertexType>& vsb)
{
	verticesCount = vsb.size();
}

inline void VertexBuffer::recreate(unsigned int sz)
{
	if(vb) release();

	sizeInBytes = sz;
	DWORD flags = D3DUSAGE_WRITEONLY;
	flags |= (type == DYNAMIC) ? D3DUSAGE_DYNAMIC : 0;
	device->CreateVertexBuffer(sizeInBytes, flags, 0, D3DPOOL_DEFAULT, &vb, 0);
}

inline void VertexBuffer::clear()
{
	if(sizeInBytes > 0)
	{
		
		release();
		sizeInBytes = 0;
		sizeOfVertex = 0;
		verticesCount = 0;
	}
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