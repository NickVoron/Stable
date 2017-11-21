#pragma once
// ограничение, можно применять к значению, можно ограничивать значение зацикленно

#include <limits>

// LOCAL:
#include "interval.h"

template<class T>
class Range	: public Base::Interval<T> {
public:
	// ctor/dtor
	inline Range(){};
	inline Range(const T& min, const T& max);
	
	// получить минимальный предел
	inline const T& GetMin() const;

	// установить минимальный уровень
	inline void SetMin(const T& value);

	// получить максимальный пределе
	inline const T& GetMax() const;

	// установить максимальный уровень
	inline void SetMax(const T& value);

	// оператор прибавление другого промежутка, получаем расширенный промежуток, включающи оба
	inline Range<T>& operator+=(const Range<T>& other);

	// инициализировать минимум для встроенного типа минимальным значением 
	inline bool InitDefaultMin();

	// иниицализировать абсолютный (положительный минимум)
	inline bool InitAbsDefaultMin();

	// инициализировать максимум для встроенного типа максимальным значением
	inline bool InitDefaultMax();

	// проверить на валидность, min < max
	inline bool IsValid() const;

	// нормализировать промежуток
	inline void Normalize();

	// лежит ли значение в промежутке?
	inline bool IsInRange(const T& value) const;

	// ограничить значение промежутком

	inline T& RangeIt(T& value) const;

	// зацикленно ограничить значение промежутком, возвращает кол-во циклов для привода значение в нормальное, знак показывает направление
	inline int CycleRangeIt(T& value) const;

	inline float GetInterpolationCoef(const T& value) const;

	bool operator==(const Range& r) const { return r.minValue == Base::Interval<T>::minValue && r.maxValue == Base::Interval<T>::maxValue; }

};

template<class T>
inline Range<T>::Range(const T& min, const T& max) : Base::Interval<T>( min, max)
{
	Normalize();
}

// получить минимальный предел
template<class T>
inline const T& Range<T>::GetMin() const
{
	return Base::Interval<T>::minValue;
}

// установить минимальный уровень
template<class T>
inline void Range<T>::SetMin(const T& value)
{
	Base::Interval<T>::minValue = value;
	Normalize();
}

// получить максимальный пределе
template<class T>
inline const T& Range<T>::GetMax() const
{
	return Base::Interval<T>::maxValue;
}

// установить максимальный уровень
template<class T>
inline void Range<T>::SetMax(const T& value)
{
	Base::Interval<T>::maxValue = value;
	Normalize();
}


// оператор прибавление другого промежутка, получаем расширенный промежуток, включающи оба
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

// инициализировать минимум для встроенного типа минимальным значением 
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

// инициализировать максимум для встроенного типа максимальным значением
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

// иниицализировать абсолютный (положительный минимум)
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


// проверить на валидность, min < max
template<class T>
inline bool Range<T>::IsValid() const
{
	return Base::Interval<T>::maxValue > Base::Interval<T>::minValue;
}

// нормализировать промежуток
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

// лежит ли значение в промежутке?
template<class T>
inline bool Range<T>::IsInRange(const T& value) const
{
	return (value >= Base::Interval<T>::minValue) && (value <= Base::Interval<T>::maxValue);
}

// ограничить значение промежутком
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
	//RangeIt(val);
	return (val - Base::Interval<T>::minValue) / (Base::Interval<T>::maxValue - Base::Interval<T>::minValue);
}

	// зацикленно ограничить значение промежутком, возвращает кол-во циклов для привода значение в нормальное, знак показывает направление
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
