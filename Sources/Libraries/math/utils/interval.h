// Copyright (C) 2012-2014 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once


namespace Base {

template < class T >
struct Interval {
public:
	inline Interval(){}
	inline Interval( const T& a, const T& b ) : minValue(a), maxValue(b) {}

	inline void set(const T& a, const T& b)
	{
		minValue = a;
		maxValue = b;
	}

	inline Interval reverse() const
	{
		return Interval(maxValue, minValue);
	}

	inline bool intersection(const Interval& i, Interval& res) const
	{
		T maxMin = std::max(i.minValue, minValue);
		T minMax = std::min(i.maxValue, maxValue);

		bool intersected = (maxMin <= minMax);
		if(intersected)
		{
			res.set(maxMin, minMax);
		}

		return intersected;
	}

	inline T center() const { return minValue + extent(); }
	inline T center(float& extent_) const { extent_ = extent(); return minValue + extent_; }
	inline T extent() const { return length() * 0.5f; }

	inline void normalize()
	{
		set( std::min(minValue, maxValue), std::max(minValue, maxValue) );
	}

	inline T length() const
	{
		T len = maxValue - minValue;
		return len >= 0 ? len : -len ;
	}











	inline T minV() const { return minValue <= maxValue ? minValue : maxValue; }
	inline T maxV() const { return minValue > maxValue ? minValue : maxValue; }

	
	T minValue;
	T maxValue;
};

template<class T>
float RemapValue( const T val, const Interval<T>& src, const Interval<T>& dst)
{
	return RemapValue(val, src.minValue, src.maxValue, dst.minValue, dst.maxValue);
}

} 




// Copyright (C) 2012-2014 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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