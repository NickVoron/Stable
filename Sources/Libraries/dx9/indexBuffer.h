// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "buffer.h"

namespace dx9
{


class IndexBuffer : public BufferBase<IndexBuffer>
{
public:
	IndexBuffer();
	~IndexBuffer();

	template<class IndexStream>	
	void upload(const IndexStream& is);

	void set() const
	{ 
		static LPDIRECT3DINDEXBUFFER9 currentIB = 0;
		
		{
			device->SetIndices(ib);
			currentIB = ib;
			incrementCounter();
		}
		
	}

	LPDIRECT3DINDEXBUFFER9 getIB() const {return ib;}

	unsigned int getIndicesCount() const{return indicesCount;}

	void clear()
	{
		if(sizeInBytes > 0)
		{
			device->SetIndices(0);
			release();
			sizeInBytes = 0;
		}
	}

	virtual void updateFromCache();

private:
	void recreate(unsigned int sizeInBytes);

	void release() {SAFE_RELEASE(ib);}


	D3DFORMAT fmt;
	LPDIRECT3DINDEXBUFFER9 ib;
	unsigned int sizeInBytes;
	unsigned int indicesCount;
};




inline IndexBuffer::IndexBuffer() : ib(0), sizeInBytes(0), fmt(D3DFMT_INDEX16), indicesCount(0)
{
}

inline IndexBuffer::~IndexBuffer()
{ 
	clear();
}

template<class IndexStream>
void IndexBuffer::upload(const IndexStream& is)
{
 	typedef typename IndexStream::Type IndexType;
 
 	fmt = sizeof(IndexType) == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32;
 
 	indicesCount = (unsigned int)is.size();
 	if(indicesCount == 0) return;
 
 	unsigned int estimatedSize = indicesCount * sizeof(IndexType);
 	if(sizeInBytes < estimatedSize)
 	{
 		recreate(estimatedSize);	
 	}
 
 	if(ib)
 	{
 		IndexType* pIndices;
 		ib->Lock(0, estimatedSize, (void**)&pIndices, (type == DYNAMIC) ? D3DLOCK_DISCARD : 0);
 		mem::memcpy(pIndices, &is[0], estimatedSize);
 		createCache(&is[0], estimatedSize);
 		ib->Unlock();
 	}
}

inline void IndexBuffer::recreate(unsigned int sz)
{
	if(ib) release();

	sizeInBytes = sz;
	DWORD flags = D3DUSAGE_WRITEONLY;
	flags |= (type == DYNAMIC) ? D3DUSAGE_DYNAMIC : 0;
	device->CreateIndexBuffer(sizeInBytes, flags, fmt, D3DPOOL_DEFAULT, &ib, 0);
}

inline void IndexBuffer::updateFromCache()
{
	if(cacheSize > 0)
		recreate(cacheSize);

	if(ib && cacheData && cacheSize)
	{
		void* dataPtr;
		DWORD flags = 0;
		flags |= (type == DYNAMIC) ? D3DLOCK_DISCARD : 0;

		ib->Lock(0, cacheSize, &dataPtr, flags);
		mem::memcpy( dataPtr, cacheData, cacheSize );
		ib->Unlock();
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