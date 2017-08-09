// Copyright (C) 2012-2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once


#include <limits>


#include "interval.h"

template<class T>
class Range	: public Base::Interval<T> {
public:
	
	inline Range(){};
	inline Range(const T& min, const T& max);
	
	
	inline const T& GetMin() const;

	
	inline void SetMin(const T& value);

	
	inline const T& GetMax() const;

	
	inline void SetMax(const T& value);

	
	inline Range<T>& operator+=(const Range<T>& other);

	
	inline bool InitDefaultMin();

	
	inline bool InitAbsDefaultMin();

	
	inline bool InitDefaultMax();

	
	inline bool IsValid() const;

	
	inline void Normalize();

	
	inline bool IsInRange(const T& value) const;

	

	inline T& RangeIt(T& value) const;

	
	inline int CycleRangeIt(T& value) const;

	inline float GetInterpolationCoef(const T& value) const;

	bool operator==(const Range& r) const { return r.minValue == Base::Interval<T>::minValue && r.maxValue == Base::Interval<T>::maxValue; }

};

template<class T>
inline Range<T>::Range(const T& min, const T& max) : Base::Interval<T>( min, max)
{
	Normalize();
}


template<class T>
inline const T& Range<T>::GetMin() const
{
	return Base::Interval<T>::minValue;
}


template<class T>
inline void Range<T>::SetMin(const T& value)
{
	Base::Interval<T>::minValue = value;
	Normalize();
}


template<class T>
inline const T& Range<T>::GetMax() const
{
	return Base::Interval<T>::maxValue;
}


template<class T>
inline void Range<T>::SetMax(const T& value)
{
	Base::Interval<T>::maxValue = value;
	Normalize();
}



template<class T>
inline Range<T>& Range<T>::operator+=(const Range<T>& other)
{
	if (other.minValue < Base::Interval<T>::minValue)
	{
		Base::Interval<T>::minValue = other.minValue;
	}
	if (other.maxValue > Base::Interval<T>::maxValue)
	{
		Base::Interval<T>::maxValue = other.maxValue;
	}
	
	return *this;
}


template<class T>
inline bool Range<T>::InitDefaultMin()
{
	if (std::numeric_limits<T>::is_specialized)
	{
		Base::Interval<T>::minValue = -(std::numeric_limits<T>::max)();
		return true;
	}
	return false;
}


template<class T>
inline bool Range<T>::InitDefaultMax()
{
	if (std::numeric_limits<T>::is_specialized)
	{
		Base::Interval<T>::maxValue = (std::numeric_limits<T>::max)();
		return true;
	}
	return false;
}


template<class T>
inline bool Range<T>::InitAbsDefaultMin()
{
	if (std::numeric_limits<T>::is_specialized)
	{
		Base::Interval<T>::minValue = (std::numeric_limits<T>::min)();
		return true;
	}
	return false;
}



template<class T>
inline bool Range<T>::IsValid() const
{
	return Base::Interval<T>::maxValue > Base::Interval<T>::minValue;
}


template<class T>
inline void Range<T>::Normalize()
{
	if (!IsValid()) 
	{
		T v = Base::Interval<T>::minValue;
		Base::Interval<T>::minValue = Base::Interval<T>::maxValue;
		Base::Interval<T>::maxValue = v;
	}
}


template<class T>
inline bool Range<T>::IsInRange(const T& value) const
{
	return (value >= Base::Interval<T>::minValue) && (value <= Base::Interval<T>::maxValue);
}


template<class T>
inline T& Range<T>::RangeIt(T& value) const
{
	if (value < Base::Interval<T>::minValue)
	{
		value = Base::Interval<T>::minValue;
	}
	if (value > Base::Interval<T>::maxValue)
	{
		value = Base::Interval<T>::maxValue;
	}

	return value;
}

template<class T>
inline float Range<T>::GetInterpolationCoef(const T& value) const
{
	T val = value;
	
	return (val - Base::Interval<T>::minValue) / (Base::Interval<T>::maxValue - Base::Interval<T>::minValue);
}

	
template<class T>
inline int Range<T>::CycleRangeIt(T& value) const
{
	int cycles = 0;
	if (value > Base::Interval<T>::maxValue)
	{
		T maxV = this->maxValue - Base::Interval<T>::minValue;
		value -= Base::Interval<T>::minValue;
		while (value > maxV) 
		{
			value -= maxV;
			cycles++;
		}
		value += Base::Interval<T>::minValue;
		return cycles;
	}

	if (value < Base::Interval<T>::minValue)
	{
		T maxV = Base::Interval<T>::maxValue - Base::Interval<T>::minValue;
		value -= Base::Interval<T>::minValue;
		while (value < 0) 
		{
			value += maxV;
			cycles--;
		}
		value += Base::Interval<T>::minValue;
		return cycles;
	}

	return 0;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const Range<T>& range)
{
	return os << "{" << range.minValue << ", " << range.maxValue << "}";
}




// Copyright (C) 2012-2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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