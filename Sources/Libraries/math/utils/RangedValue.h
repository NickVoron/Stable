// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once


#include "Range.h"

template <class T>
class RangedValue: private Range<T>
{
public:
	
	RangedValue();
	RangedValue(const T& value);
	RangedValue(const T& min, const T& max);
	RangedValue(const T& min, const T& max, const T& value);
		
	
	inline const T& GetCurrentValue() const;

	
	inline void SetCurrentValue(const T& value);

	
	inline bool IsCurrentMin() const;

	
	inline bool IsCurrentMax() const;

	
	inline operator const T&() const;

	
	inline const T& operator=(const T& value);

	
	inline const T& operator+=(const T& value);

	
	inline const T& operator-=(const T& value);

	
	inline const T& operator*=(const T& value);

	
	inline const T& operator/=(const T& value);
																 
	
	inline bool operator==(const RangedValue<T>& rhs);

	
	inline void ValidateValue();

	
	
	inline const T& GetMin() const;

	
	inline void SetMin(const T& value);

	
	inline const T& GetMax() const;

	
	inline void SetMax(const T& value);

	
	inline bool IsInRange(const T& value) const;

	
	inline T& RangeIt(T& value) const;

	
	inline int CycleRangeIt(T& value) const;

	
	inline void SetCycleMode(bool flag);

	
	inline bool IsCycleMode() const;

protected:
	T		curValue;	
	bool	needCycle;	
};


template<class T>
RangedValue<T>::RangedValue(): curValue(0)
{
	needCycle = false;
}

template<class T>
RangedValue<T>::RangedValue(const T& value): curValue(value)
{
	needCycle = false;
}

template<class T>
RangedValue<T>::RangedValue(const T& min, const T& max): Range<T>(min, max)
{
	needCycle = false;
}

template<class T>
RangedValue<T>::RangedValue(const T& min, const T& max, const T& value): Range<T>(min, max), curValue(value)
{
	needCycle = false;
}


template<class T>
inline const T& RangedValue<T>::GetCurrentValue() const
{
	return curValue;
}


template<class T>
inline void RangedValue<T>::SetCurrentValue(const T& value)
{
	curValue = value;
	ValidateValue();
}


template<class T>
inline bool RangedValue<T>::IsCurrentMin() const
{
	return curValue == this->minValue;
}


template<class T>
inline bool RangedValue<T>::IsCurrentMax() const
{
	return curValue == this->maxValue;
}


template<class T>
inline RangedValue<T>::operator const T&() const
{
	return curValue;
}


template<class T>
inline const T& RangedValue<T>::operator+=(const T& value)
{
	SetCurrentValue(curValue + value);
	return curValue;
}


template<class T>
inline const T& RangedValue<T>::operator-=(const T& value)
{
	SetCurrentValue(curValue - value);
	return curValue;
}


template<class T>
inline const T& RangedValue<T>::operator*=(const T& value)
{
	SetCurrentValue(curValue * value);
	return curValue;
}


template<class T>
inline const T& RangedValue<T>::operator/=(const T& value)
{
	SetCurrentValue(curValue / value);
	return curValue;
}



template<class T>
inline const T& RangedValue<T>::operator=(const T& value)
{
	SetCurrentValue(value);
	return curValue;
}


template<class T>
bool RangedValue<T>::operator==(const RangedValue<T>& rhs)
{
	return rhs.minValue == Base::Interval<T>::minValue && rhs.maxValue == Base::Interval<T>::maxValue && rhs.curValue == curValue;
}


template<class T>
inline void RangedValue<T>::ValidateValue()
{
	if (!needCycle) 
	{
		Range<T>::RangeIt(curValue);
	}
	else
	{
		Range<T>::CycleRangeIt(curValue);
	}
}


template<class T>
inline const T& RangedValue<T>::GetMin() const
{
	return Range<T>::GetMin();
}


template<class T>
inline void RangedValue<T>::SetMin(const T& value)
{
	Range<T>::SetMin(value);
	ValidateValue();
}


template<class T>
inline const T& RangedValue<T>::GetMax() const
{
	return Range<T>::GetMax();
}


template<class T>
inline void RangedValue<T>::SetMax(const T& value)
{
	Range<T>::SetMax(value);
	ValidateValue();
}


template<class T>
inline bool RangedValue<T>::IsInRange(const T& value) const
{
	return Range<T>::IsInRange(value);
}


template<class T>
inline T& RangedValue<T>::RangeIt(T& value) const
{
	return Range<T>::RangeIt(value);
}


template<class T>
inline int RangedValue<T>::CycleRangeIt(T& value) const
{
	return Range<T>::CycleRangeIt(value);
}


template<class T>
inline void RangedValue<T>::SetCycleMode(bool flag)
{
	needCycle = flag;
}


template<class T>
inline bool RangedValue<T>::IsCycleMode() const
{
	return needCycle;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const RangedValue<T>& range)
{
	return os << "{ val: " << range.GetCurrentValue() << " min: " << range.GetMin() << ", max: " << range.GetMax() << "}";
}




// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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