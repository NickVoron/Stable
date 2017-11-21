#pragma once

#include "newmath/library.include.h"
#include "math/library.include.h"


namespace image
{
	template<typename T> struct accumulation_type;
	template<> struct accumulation_type<float>			{ typedef float result; };
	template<> struct accumulation_type<double>			{ typedef double result; };
	template<> struct accumulation_type<half_float>		{ typedef float result; };
	template<> struct accumulation_type<signed short>	{ typedef float result; };
	template<> struct accumulation_type<unsigned short> { typedef float result; };
	template<> struct accumulation_type<signed char>	{ typedef float result; };
	template<> struct accumulation_type<unsigned char>	{ typedef float result; };

	template<typename T> class ChannelAccessor;

	template<typename T> struct max_value { static inline float value() { return (float)std::numeric_limits<T>::max();}	static inline signed int tvalue() { return std::numeric_limits<T>::max();} };
	template<typename T> struct min_value { static inline float value() { return (float)std::numeric_limits<T>::min();}	static inline signed int tvalue() { return std::numeric_limits<T>::min();} };

// 	template<> struct max_value<signed int>		{ static inline float value() { return 32767.0f;}	static inline signed int		tvalue() { return 32767;}	};
// 	template<> struct max_value<signed short>	{ static inline float value() { return 32767.0f;}	static inline signed short		tvalue() { return 32767;}	};
// 	template<> struct max_value<signed char>	{ static inline float value() { return 127.0f;}		static inline signed char		tvalue() { return 127;}		};
// 
// 	template<> struct max_value<unsigned int>	{ static inline float value() { return 65535.0f;}	static inline unsigned int		tvalue() { return 65535;}	};
// 	template<> struct max_value<unsigned short> { static inline float value() { return 65535.0f;}	static inline unsigned short	tvalue() { return 65535;}	};
// 	template<> struct max_value<unsigned char>	{ static inline float value() { return 255.0f;}		static inline unsigned char		tvalue() { return 255;}		};
// 
// 	
// 	template<> struct min_value<signed int>		{ static inline float value() { return -32767.0f;}	static inline signed int		tvalue() { return -32767;}	};
// 	template<> struct min_value<signed short>	{ static inline float value() { return -32767.0f;}	static inline signed short		tvalue() { return -32767;}	};
// 	template<> struct min_value<signed char>	{ static inline float value() { return -127.0f;}	static inline signed char		tvalue() { return -127;}	};
// 
// 	template<> struct min_value<unsigned int>	{ static inline float value() { return 0.0f;}		static inline unsigned int		tvalue() { return 0;}		};
// 	template<> struct min_value<unsigned short> { static inline float value() { return 0.0f;}		static inline unsigned short	tvalue() { return 0;}		};
// 	template<> struct min_value<unsigned char>	{ static inline float value() { return 0.0f;}		static inline unsigned char		tvalue() { return 0;}		};

	template<> struct max_value<char> : public max_value<signed char> {};
	template<> struct min_value<char> : public min_value<signed char> {};

	template<typename T>	inline T			floatClamp(float value) { return (T)nm::clamp(value, min_value<T>::value(), max_value<T>::value()); }
	template<> 				inline half_float	floatClamp<half_float>	(float value)	{ return value; }
	template<> 				inline float		floatClamp<float>		(float value)	{ return value; }
	template<> 				inline double		floatClamp<double>		(float value)	{ return (float)value; }

	template<typename T>	inline float mapToFloat				(T value)			{ return value / max_value<T>::value(); }
	template<> 				inline float mapToFloat<half_float>	(half_float value)	{ return value; }
	template<> 				inline float mapToFloat<float>		(float value)		{ return value; }
	template<> 				inline float mapToFloat<double>		(double value)		{ return (float)value; }

	template<typename T>	inline T			mapFromFloat				(float value) { return floatClamp<T>( value * max_value<T>::value() ); }
	template<> 				inline half_float	mapFromFloat<half_float>	(float value) { return value; }
	template<> 				inline float		mapFromFloat<float>			(float value) { return value; }
	template<> 				inline double		mapFromFloat<double>		(float value) { return (double)value; }


	template<typename T0, typename T1>
	struct normalizedConvertorImpl { static inline T0 convert(T1 value) { return mapFromFloat<T0>(mapToFloat(value)); } };

	template<typename T>
	struct normalizedConvertorImpl<T, T> { static inline T convert(T value) { return value; } };

	template<typename T0, typename T1>
	struct normalizedConvertor 
	{
		static inline T0 convert(T1 value)	{ return normalizedConvertorImpl<T0, T1>::convert(value);	}
	};

	template<typename T0, typename T1>	inline T0 normalizedConvert(T1 value)	
	{ 
		return normalizedConvertor<T0, T1>::convert(value);
	}

	template<typename T, typename T1> inline void mappedAdd(T& res, const T1& val) { res = mapFromFloat<T>( mapToFloat(res) + mapToFloat(val) ); }
	template<typename T, typename T1> inline void mappedSub(T& res, const T1& val) { res = mapFromFloat<T>( mapToFloat(res) - mapToFloat(val) ); }
	template<typename T, typename T1> inline void mappedMul(T& res, const T1& val) { res = mapFromFloat<T>( mapToFloat(res) * mapToFloat(val) ); }
	template<typename T, typename T1> inline void mappedDiv(T& res, const T1& val) { res = mapFromFloat<T>( mapToFloat(res) / mapToFloat(val) ); }
	template<typename T, typename T1> inline void mappedSet(T& res, const T1& val) { res = mapFromFloat<T>( mapToFloat(val) ); }

	template<typename T, typename T1> inline void mappedAdd(ChannelAccessor<T>& res, const T1& val) { res = mapFromFloat<T>( mapToFloat(res.val) + mapToFloat(val) ); }
 	template<typename T, typename T1> inline void mappedSub(ChannelAccessor<T>& res, const T1& val) { res = mapFromFloat<T>( mapToFloat(res.val) - mapToFloat(val) ); }
 	template<typename T, typename T1> inline void mappedMul(ChannelAccessor<T>& res, const T1& val) { res = mapFromFloat<T>( mapToFloat(res.val) * mapToFloat(val) ); }
 	template<typename T, typename T1> inline void mappedDiv(ChannelAccessor<T>& res, const T1& val) { res = mapFromFloat<T>( mapToFloat(res.val) / mapToFloat(val) ); }
	template<typename T, typename T1> inline void mappedSet(ChannelAccessor<T>& res, const T1& val) { res = mapFromFloat<T>( mapToFloat(val) ); }

	template<typename T>
	class ChannelAccessor
	{
	public:
		inline ChannelAccessor(T& v):val(v){}
		
		template<typename T1>
		inline ChannelAccessor& operator=(const ChannelAccessor<T1>& t)	{ (*this) = t.val;			return *this;	}

		inline ChannelAccessor& operator=(const ChannelAccessor& t)		{ val = t.val;				return *this;	}
		inline ChannelAccessor& operator=(const T& t)					{ val = t;					return *this;	}
		inline ChannelAccessor& operator=(float t)						{ val = mapFromFloat<T>(t);	return *this;	}
		inline ChannelAccessor& operator=(double t)						{ val = mapFromFloat<T>(t);	return *this;	}
		inline ChannelAccessor& operator=(half_float t)					{ val = mapFromFloat<T>(t);	return *this;	}

		inline operator T() const	{return val;}
		inline operator T&()		{return val;}

		T& val;
	};

	template<>
	class ChannelAccessor<float>
	{
	public:
		inline ChannelAccessor(float& v):val(v){}
		inline ChannelAccessor& operator=(float t)	{ val = t;	return *this;	}
		inline ChannelAccessor& operator=(const ChannelAccessor& t)		{ val = t.val; return *this;	}

		template<typename T1>
		inline ChannelAccessor& operator=(const ChannelAccessor<T1>& t)	{ (*this) = mapToFloat(t.val);	return *this;	}
		
		inline operator float() const {return val;}
		inline operator float&() {return val;}

		float& val;
	};

	//
	//
	//
	template<typename T> struct is_float	{ static const bool value = false; };
	template<> struct is_float<half_float>	{ static const bool value = true; };
	template<> struct is_float<float>		{ static const bool value = true; };
	template<> struct is_float<double>		{ static const bool value = true; };

	template<typename T> void saturate(T& val) { val = max_value<T>::tvalue(); };
	template<typename T> void saturate(ChannelAccessor<T>& val) { val.val = max_value<T>::tvalue(); };
	
}
