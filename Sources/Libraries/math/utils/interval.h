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

// 	inline float relative(T val) const
// 	{
// 		return (val - minV()) / (float)length();
// 	}
// 
// 	inline float absolute(T val) const
// 	{
// 		return (val - minV()) / (float)length();
// 	}

	inline T minV() const { return minValue <= maxValue ? minValue : maxValue; }
	inline T maxV() const { return minValue > maxValue ? minValue : maxValue; }

	// data:
	T minValue;
	T maxValue;
};

template<class T>
float RemapValue( const T val, const Interval<T>& src, const Interval<T>& dst)
{
	return RemapValue(val, src.minValue, src.maxValue, dst.minValue, dst.maxValue);
}

} // -namespace
