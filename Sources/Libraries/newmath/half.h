// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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




// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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