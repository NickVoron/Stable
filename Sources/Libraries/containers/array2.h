// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <boost/type_traits.hpp>

#include "stuff/nonCopyable.h"
#include "newmath/library.include.h"
#include "memory/library.include.h"

namespace Base 
{

template< class T >
struct Array2DataBase
{
	Array2DataBase()  { Array2DataBase::Defaults(); }

	void Defaults()
	{
		data = 0;
		size.x = size.y = 0;
	}

	T* data;
	nm::index2 size;
};

template< class T, bool typeIsPOD > struct Array2Data;

template< class T >
struct Array2Data<T, true> : public Array2DataBase<T>
{
	~Array2Data()
	{
		Clear();
	}

	void ZeroMem()
	{
		unsigned int s = this->size.Square() * sizeof(T);
		if(s > 0)
		{
			mem::memset(this->data, 0, s );
		}	
	}

	void Clear()
	{
		if (this->data)
			free(this->data);
		this->Defaults();
	}

	void Generate( const nm::index2& newSize )
	{
		Clear();
		unsigned int s = newSize.Square() * sizeof(T);
		this->data = (T*)malloc(s);
		this->size = newSize;
	}
};

template< class T >
struct Array2Data<T, false> : public Array2DataBase<T>
{
	~Array2Data()
	{
		Clear();
	}

	void Clear()
	{
		delete [] this->data;
		this->Defaults();
	}

	void Generate( const nm::index2& newSize )
	{
		Clear();
		this->data = new T[newSize.Square()];
		this->size = newSize;
	}
};

template< class T >
class Array2 :	public Array2Data<T,	boost::has_trivial_copy<T>::value && 
										boost::has_trivial_destructor<T>::value && 
										boost::has_trivial_constructor<T>::value >, 
				public NonCopyable 
{
public:
	const	T& operator[]( const nm::index2& i ) const { return this->data[ i.x + i.y * this->size.x ]; }
			T& operator[]( const nm::index2& i ) { return this->data[ i.x + i.y * this->size.x ]; }

	const	T& AcessCyclic( const nm::index2& i ) const { return this->data[ i.x % this->size.x + ( i.y % this->size.y ) * this->size.x ]; }
			T& AcessCyclic( const nm::index2& i ) { return this->data[ i.x % this->size.x + ( i.y % this->size.y ) * this->size.x ]; }
	
	const	T& AcessFlat( int flatIdx ) const { return this->data[ flatIdx ]; }
			T& AcessFlat( int flatIdx ) { return this->data[ flatIdx ]; }
	
	
	bool IsEmpty() const { return this->size == nm::index2::outIndex; }
	
	const nm::index2& GetSize() const { return this->size; }
	bool IsContain( const nm::index2& i ) const { return ( (i.x>=0) && (i.y>=0) && (i.x < this->size.x) && (i.y < this->size.y) ); }
	T* GetData() const { return this->data; }
	unsigned int GetBinarySize() const { return sizeof(T) * this->size.x * this->size.y; }

	
	void CopyFrom( const Array2<T>& sourceArray );
	void SetAllElements( const T& value );
};



template< class T >
void
Array2<T>::CopyFrom( const Array2<T>& sourceArray )
{
	Generate( sourceArray.GetSize() );
	mem::memcpy(this->data, sourceArray.data, this->size.Square() * sizeof(T) );
}

template< class T >
void
Array2<T>::SetAllElements( const T& value )
{
	if (this->data == 0 ) return;

	nm::index2 it;
	for ( it.y = 0; it.y < this->size.y; it.y++ )
		for ( it.x = 0; it.x < this->size.x; it.x++ )
			operator[]( it ) = value;
}

template<class T>
T calculateArray2AverageValue(const Array2<T>& arr)
{
	T val;
	mem::memzero(val);

	nm::index2 it;
	nm::index2 size = arr.GetSize();
	for ( it.y = 0; it.y < size.y; it.y++ )
	{
		for ( it.x = 0; it.x < size.x; it.x++ )
		{
			val += arr[it];
		}
	}

	val /= (float)size.Square();

	return val;
}



} // namespace Base



// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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