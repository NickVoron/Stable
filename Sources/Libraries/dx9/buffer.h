// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "stuff/library.include.h"
#include "stream/library.include.h"
#include "containers/library.include.h"

#include "dx9/interfaces.h"

namespace dx9
{

class Buffer
{
public:
	enum BufferType	{ STATIC, DYNAMIC, };

	inline Buffer():type(STATIC), cacheSize(0), cacheData(0){}
	inline ~Buffer(){ clearCache(); }
	inline void setType(BufferType t){type = t;}

	void save(stream::ostream& os);
	void load(stream::istream& is);

	virtual void updateFromCache() = 0;

protected:
	inline void clearCache()
	{
		if(cacheData) free(cacheData);
		cacheSize = 0;
	}

	inline void createCache(const void* src, unsigned int sz)
	{
		allocCache(sz);
		mem::memcpy(cacheData, src, sz);
	}

	inline void allocCache(unsigned int sz)
	{
		if(cacheSize < sz)
		{
			clearCache();
			cacheData = malloc(sz);
			cacheSize = sz;
		}
	}

	BufferType type;
	void* cacheData;
	unsigned int cacheSize;
};

template<class T>
class BufferBase : public Buffer, public Base::IntrusiveList<T>::Node
{
public:
	inline BufferBase() { BuffersHolder<T>::insert(*this); }
  	inline ~BufferBase() { this->remove(); }

	static inline void clearAll()				{ BuffersHolder<T>::clearAll();}
	static inline void updateAllFromCache()	{ BuffersHolder<T>::updateAllFromCache();}

	static int changeCounter;
	static inline void resetCounter(){changeCounter = 0;}
	static inline void incrementCounter() {++changeCounter;}
};


template<class BufferClass>
struct BuffersHolder : public Base::IntrusiveList<BufferClass>, public Base::MeyersSingleton< BuffersHolder<BufferClass> > 
{
	template<class T>
	static void insert(BufferBase<T>& buffer)
	{
		Base::MeyersSingleton< BuffersHolder<BufferClass> >::get().push_back((T&)buffer);
	}

	static void clearAll()
	{
		BufferClass* e = get().end();
		for (BufferClass* buffer = get().first(); buffer != e; buffer->iterate(buffer))
		{
			buffer->clear();
		}
		get().clear();
	}

	static void updateAllFromCache()
	{
		int size = get().size();
		BufferClass* e = get().end();
		for (BufferClass* buffer = get().first(); buffer != e; buffer->iterate(buffer))
		{
			buffer->updateFromCache();
		}
	}
};

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