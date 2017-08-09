// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <stdexcept>

#include "memory.h"

namespace mem
{
	template<std::size_t length>
	class membuf
	{
	public:

		typedef membuf type;

		enum { LENGTH = length };

		membuf();
		membuf(void* data, std::size_t len);
		~membuf();

		
		
		
		void insert(const void* d, std::size_t len);
		template<class V> void insert(const V& value);
		template<std::size_t len> void insert(const membuf<len>& buf);


		
		
		
		template<class V> V* take();

		virtual void takeCopy(void* d, std::size_t len );
		template<class V> void takeCopy(V& v);
		template<std::size_t len> void takeCopy( membuf<len>& buf );
		
		
		void clear();
		void fill(char val);


		
		
		
		std::size_t getSize() const;
	 	void setSize(std::size_t size);
		static std::size_t getLength() {return LENGTH;}

		void* getEndPtr() {return (char*)data() + cursor;}

		
		std::size_t getUsed() const;
		std::size_t getFree() const;

		
		mem::mem_desc getDataDesc();

		
		bool canContain(std::size_t size) const;
		bool isContain(std::size_t size) const;
		bool isEmpty() const;

	private:
 		void* takeData(std::size_t len);
		void* data();
		const void* data() const;

		size_t cursor;

		char* dataPtr__;
		char data__[LENGTH];
	};



template<std::size_t length>
membuf<length>::membuf():cursor(0), dataPtr__(&data__[0]){};

template<std::size_t length>
membuf<length>::membuf(void* data, std::size_t len):cursor(0), dataPtr__(&data__[0]){ insert(data, len); }

template<std::size_t length>
membuf<length>::~membuf(){};


template<std::size_t length>
void membuf<length>::insert(const void* d, std::size_t len)
{
	if( canContain(len) ) 
	{
		memcpy(dataPtr__ + cursor, d, len);
		cursor += len;
	}
	else
	{
		throw std::runtime_error("membuf::insert(): not enough space");
	}
}

template<std::size_t length>
template<class V>
void membuf<length>::insert(const V& value)
{
	insert( (void*)&value, sizeof(V));
}

template<std::size_t length>
template<std::size_t len>
void membuf<length>::insert(const membuf<len>& buf)
{
	insert(buf.data(), buf.getUsed());
}


template<std::size_t length>
template<class V>
V* membuf<length>::take()
{
	return takeData(sizeof(V));
}


template<std::size_t length>
template< class V>
void membuf<length>::takeCopy(V& v)
{
	void* cdp = takeData(sizeof(V));
	if(cdp)
	{
		mem::memcpy(&v, cdp, sizeof(V));
	}
}


template<std::size_t length>
template<std::size_t len>
void membuf<length>::takeCopy( membuf<len>& buf )
{
	void* cdp = takeData(len);
	if(cdp)
	{
		buf.clear();
		buf.insert(cdp, len);
	}
}


template<std::size_t length>
void* membuf<length>::data() 
{
	return dataPtr__;
}

template<std::size_t length>
const void* membuf<length>::data() const 
{
	return dataPtr__;
}

template<std::size_t length>
void membuf<length>::clear()
{
	cursor  = 0;
	dataPtr__ = data__;
}

template<std::size_t length>
void membuf<length>::fill(char val)
{
	memset(data(), val, LENGTH);
}

template<std::size_t length>
std::size_t membuf<length>::getSize() const {return cursor;}

template<std::size_t length>
void membuf<length>::setSize(std::size_t size)
{
	if( size <= LENGTH ) 
	{
		cursor = size;
	}
	else
	{
		throw std::runtime_error("membuf::setSize(): not enough space");
	}
}

template<std::size_t length>
std::size_t membuf<length>::getUsed() const
{
	return cursor;
}

template<std::size_t length>
std::size_t membuf<length>::getFree() const
{
	return LENGTH - cursor;
}

template<std::size_t length>
mem::mem_desc membuf<length>::getDataDesc() 
{
	return mem::mem_desc(data(), cursor); 
}

template<std::size_t length>
bool membuf<length>::canContain(std::size_t size) const
{
	return getFree() >= size;
}

template<std::size_t length>
bool membuf<length>::isContain(std::size_t size)  const
{
	return getUsed() >= size;
}

template<std::size_t length>
bool membuf<length>::isEmpty() const
{
	return cursor == 0;
}
	

template<std::size_t length>
void* membuf<length>::takeData(std::size_t len)
{
	if(isContain(len))
	{
		cursor -= len;				
		return (char*)data() + cursor;
	}
	else
	{
		throw std::runtime_error("membuf::take(): not enough data");
	}

	return 0;			
}

template<std::size_t length>
void membuf<length>::takeCopy(void* d, std::size_t len )
{
	void* ptr = takeData(len);
	mem::memcpy(d, ptr, len);
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