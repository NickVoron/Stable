#pragma once

#include "stream/library.include.h"

#include "fixedArray.h"

namespace Base
{

template<class T, const int maxCount>
stream::ostream& operator<<(stream::ostream& os, const FixedArray<T, maxCount>& arr)
{
	os << arr.size();
	if(arr.size() > 0)
	{
		os.write(&arr[0], arr.size() * sizeof(T));
	}	
	return os;
}

template<class T, const int maxCount>
stream::istream& operator>>(stream::istream& is, FixedArray<T, maxCount>& arr)
{
	int sz; is >> sz;
	if(sz > 0)
	{
		arr.resize(sz);
		is.read(&arr[0], arr.size() * sizeof(T));
	}
	else
	{
		arr.clear();
	}
	
	return is;
}

}

