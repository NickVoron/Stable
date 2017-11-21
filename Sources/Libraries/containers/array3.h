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

	// operations:
	void Generate( const nm::index3& newSize );
	T& operator[]( const nm::index3& i ) 
	{
		return data[ i.x + i.y * size.x + i.z*size.x*size.y]; 
	}
	T& AcessCyclic( const nm::index3& i ) 
	{ 
		return data[ i.x % size.x + ( i.y % size.y ) * size.x + ( i.z % size.z )*size.x*size.y ];
	}
	
	// selectors:
	void IsEmpty() const { return size == nm::index3::outIndex; }
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

	// utility:
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