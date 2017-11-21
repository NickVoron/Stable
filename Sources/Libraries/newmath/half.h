#pragma once

#include <cstddef>

class half_float
{
public:
	inline half_float(){}
	inline half_float(float val){ h = float_to_half::convert(val); }
	inline operator float()	{ return half_to_float::convert(h); }

	inline void operator+=(half_float val) { h = float_to_half::convert( half_to_float::convert(h) + half_to_float::convert(val.h) ); }
	inline void operator-=(half_float val) { h = float_to_half::convert( half_to_float::convert(h) - half_to_float::convert(val.h) ); }
	inline void operator*=(half_float val) { h = float_to_half::convert( half_to_float::convert(h) * half_to_float::convert(val.h) ); }
	inline void operator/=(half_float val) { h = float_to_half::convert( half_to_float::convert(h) / half_to_float::convert(val.h) ); }

	inline half_float operator+(half_float val) const { half_float v = (*this); v += val; return v; }
	inline half_float operator-(half_float val) const { half_float v = (*this); v -= val; return v; }
	inline half_float operator*(half_float val) const { half_float v = (*this); v *= val; return v; }
	inline half_float operator/(half_float val) const { half_float v = (*this); v /= val; return v; }

	inline void operator+=(float val) { h = float_to_half::convert( half_to_float::convert(h) + val ); }
	inline void operator-=(float val) { h = float_to_half::convert( half_to_float::convert(h) - val ); }
	inline void operator*=(float val) { h = float_to_half::convert( half_to_float::convert(h) * val ); }
	inline void operator/=(float val) { h = float_to_half::convert( half_to_float::convert(h) / val ); }

	inline half_float operator+(float val) const { half_float v = (*this); v += val; return v; }
	inline half_float operator-(float val) const { half_float v = (*this); v -= val; return v; }
	inline half_float operator*(float val) const { half_float v = (*this); v *= val; return v; }
	inline half_float operator/(float val) const { half_float v = (*this); v /= val; return v; }


	struct half_to_float
	{
		struct mantissa
		{
			mantissa();
			static unsigned int convert(unsigned int i);
			unsigned int table[2028];
			inline unsigned int operator[](std::size_t i) const {return table[i];}
		};

		struct exponent 
		{
			exponent();
			unsigned int table[64];
			inline unsigned int operator[](std::size_t i) const {return table[i];}
		};

		struct offset 
		{
			offset();
			unsigned short table[64];
			inline unsigned short operator[](std::size_t i) const {return table[i];}
		};

		static float convert(unsigned short h);
	};

	struct float_to_half
	{
		struct tables
		{
			tables();
			unsigned short base[512];
			unsigned char shift[512];
		};

		static unsigned short convert(float f_);
	};

private:
	unsigned short h;
};
