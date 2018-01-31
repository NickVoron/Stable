// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "newmath/index3.h"
#include "stuff/nonCopyable.h"

namespace Base {

template< class T >
class Array3 : public NonCopyable {
public:
	Array3() { Array3<T>::Defaults(); }
	virtual ~Array3() { Array3<T>::Clear(); }
	void Clear();
	void ZeroMem();

	
	void Generate( const nm::index3& newSize );
	T& operator[]( const nm::index3& i ) 
	{
		return data[ i.x + i.y * size.x + i.z*size.x*size.y]; 
	}
	T& AcessCyclic( const nm::index3& i ) 
	{ 
		return data[ i.x % size.x + ( i.y % size.y ) * size.x + ( i.z % size.z )*size.x*size.y ];
	}
	
	
	bool IsEmpty() const { return size == nm::index3::outIndex; }
	const T& operator[]( const nm::index3& i ) const 
	{ 
		return data[ i.x + i.y * size.x + i.z*size.x*size.y]; 
	}
	const T& AcessCyclic( const nm::index3& i ) const 
	{ 
		return data[ i.x % size.x + ( i.y % size.y ) * size.x +( i.z % size.z )*size.x*size.y ];
	}

	const nm::index3& GetSize() const { return size; }
	bool IsContain( const nm::index3& i ) const 
	{
		return ( (i.x>=0) && (i.y>=0)&& (i.z>=0) && (i.x < size.x) && (i.y < size.y) && (i.z < size.z)); 
	}
	T* GetData() const { return data; }
	int GetBinarySize() const { return sizeof(T) * size.x * size.y * size.z; }

	
	void CopyFrom( const Array3<T>& sourceArray );
	void SetAllElements( const T& value );
	void TrowIfNotContains( const nm::index3& i ) { if ( ! IsContain( i ) ) throw Base::Errors::Simple("Array3: out of range"); }


private:
	T* data;
	void* _data;
	nm::index3 size;

	void Defaults();		
};



template< class T >
void
Array3<T>::Defaults()
{
	data = 0;
	_data = 0;
	size = nm::index3::outIndex;
}



template< class T >
void
Array3<T>::Clear()
{
	if ( _data )
		free(_data);
	Defaults();
}

template< class T >
void Array3<T>::ZeroMem()
{
	memsets( data, 0, size.Square() * sizeof( T ) );
}



template< class T >
void Array3<T>::Generate( const nm::index3& newSize )
{
	if ( newSize.x <= 0 || newSize.y <= 0 || newSize.z <= 0 || newSize.x > 4096 || newSize.y > 4096 || newSize.z > 4096)	
		throw Base::Errors::Simple("Array3::Generate(): out of range");

	_data = malloc( newSize.Square() * sizeof(T) );
	data = reinterpret_cast<T*>(_data);
	size = newSize;
}

template< class T > 
void Array3<T>::CopyFrom( const Array3<T>& sourceArray )
{
	if ( sourceArray.IsEmpty() )
		return;

	mem::memcpy( data, sourceArray.data, size.Square() * sizeof(T) );
}

template< class T >
void Array3<T>::SetAllElements( const T& value )
{
	if ( data == 0 ) return;

	T* t = data;
	for(int i=0; i< size.Square(); i++, t++)
	{
		(*t) = value;
	
	}

}


} // namespace Base`



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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