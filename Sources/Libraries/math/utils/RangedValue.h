#pragma once
// значение с ограничением, может быть зацикленно по кругу

#include "Range.h"

template <class T>
class RangedValue: private Range<T>
{
public:
	// ctor/dtor
	RangedValue();
	RangedValue(const T& value);
	RangedValue(const T& min, const T& max);
	RangedValue(const T& min, const T& max, const T& value);
		
	// получить текущее значение
	inline const T& GetCurrentValue() const;

	// установить текущее значение
	inline void SetCurrentValue(const T& value);

	// текущее значение минимально?
	inline bool IsCurrentMin() const;

	// текущее значение максимально?
	inline bool IsCurrentMax() const;

	// оператор получения текущего значения
	inline operator const T&() const;

	// оператор установки текущего значения
	inline const T& operator=(const T& value);

	// унарный оператор +
	inline const T& operator+=(const T& value);

	// унарный оператор -
	inline const T& operator-=(const T& value);

	// унарный оператор *
	inline const T& operator*=(const T& value);

	// унарный оператор /
	inline const T& operator/=(const T& value);
																 
	// оператор сравнения
	inline bool operator==(const RangedValue<T>& rhs);

	// обновить текущее значение, чтоб лежало в пределах промежутка
	inline void ValidateValue();

	// дублирование интерфейса предела
	// получить минимальный предел
	inline const T& GetMin() const;

	// установить минимальный уровень
	inline void SetMin(const T& value);

	// получить максимальный пределе
	inline const T& GetMax() const;

	// установить максимальный уровень
	inline void SetMax(const T& value);

	// лежит ли значение в промежутке?
	inline bool IsInRange(const T& value) const;

	// ограничить значение промежутком
	inline T& RangeIt(T& value) const;

	// зацикленно ограничить значение промежутком
	inline int CycleRangeIt(T& value) const;

	// установить признак зацикливания значения
	inline void SetCycleMode(bool flag);

	// получить признак зацикливания значения
	inline bool IsCycleMode() const;

protected:
	T		curValue;	// текущее значение
	bool	needCycle;	// признак зацикливания значения
};

// ctor/dtor
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

// получить текущее значение
template<class T>
inline const T& RangedValue<T>::GetCurrentValue() const
{
	return curValue;
}

// установить текущее значение
template<class T>
inline void RangedValue<T>::SetCurrentValue(const T& value)
{
	curValue = value;
	ValidateValue();
}

// текущее значение минимально?
template<class T>
inline bool RangedValue<T>::IsCurrentMin() const
{
	return curValue == this->minValue;
}

// текущее значение максимально?
template<class T>
inline bool RangedValue<T>::IsCurrentMax() const
{
	return curValue == this->maxValue;
}

// оператор получения текущего значения
template<class T>
inline RangedValue<T>::operator const T&() const
{
	return curValue;
}

// унарный оператор +
template<class T>
inline const T& RangedValue<T>::operator+=(const T& value)
{
	SetCurrentValue(curValue + value);
	return curValue;
}

// унарный оператор -
template<class T>
inline const T& RangedValue<T>::operator-=(const T& value)
{
	SetCurrentValue(curValue - value);
	return curValue;
}

// унарный оператор *
template<class T>
inline const T& RangedValue<T>::operator*=(const T& value)
{
	SetCurrentValue(curValue * value);
	return curValue;
}

// унарный оператор /
template<class T>
inline const T& RangedValue<T>::operator/=(const T& value)
{
	SetCurrentValue(curValue / value);
	return curValue;
}


// оператор установки текущего значения
template<class T>
inline const T& RangedValue<T>::operator=(const T& value)
{
	SetCurrentValue(value);
	return curValue;
}

// оператор сравнения
template<class T>
bool RangedValue<T>::operator==(const RangedValue<T>& rhs)
{
	return rhs.minValue == Base::Interval<T>::minValue && rhs.maxValue == Base::Interval<T>::maxValue && rhs.curValue == curValue;
}

// обновить текущее значение, чтоб лежало в пределах промежутка
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

// получить минимальный предел
template<class T>
inline const T& RangedValue<T>::GetMin() const
{
	return Range<T>::GetMin();
}

// установить минимальный уровень
template<class T>
inline void RangedValue<T>::SetMin(const T& value)
{
	Range<T>::SetMin(value);
	ValidateValue();
}

// получить максимальный пределе
template<class T>
inline const T& RangedValue<T>::GetMax() const
{
	return Range<T>::GetMax();
}

// установить максимальный уровень
template<class T>
inline void RangedValue<T>::SetMax(const T& value)
{
	Range<T>::SetMax(value);
	ValidateValue();
}

// лежит ли значение в промежутке?
template<class T>
inline bool RangedValue<T>::IsInRange(const T& value) const
{
	return Range<T>::IsInRange(value);
}

// ограничить значение промежутком
template<class T>
inline T& RangedValue<T>::RangeIt(T& value) const
{
	return Range<T>::RangeIt(value);
}

// зацикленно ограничить значение промежутком
template<class T>
inline int RangedValue<T>::CycleRangeIt(T& value) const
{
	return Range<T>::CycleRangeIt(value);
}

// установить признак зацикливания значения
template<class T>
inline void RangedValue<T>::SetCycleMode(bool flag)
{
	needCycle = flag;
}

// получить признак зацикливания значения
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
