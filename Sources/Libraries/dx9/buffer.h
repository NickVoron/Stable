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

//
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