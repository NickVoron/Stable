// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <boost/static_assert.hpp>
#include <algorithm>

#pragma pack(push,1)

namespace image
{



template<typename T, unsigned int n_planes, unsigned int a_bits, unsigned int a_pos, unsigned int r_bits, unsigned int r_pos, unsigned int g_bits, unsigned int g_pos, unsigned int b_bits, unsigned int b_pos> 
struct IntColor
{
	T val;

	T value(unsigned int pos, unsigned int bits) const
	{
		return (val >> pos) & ((1<<bits)-1);
	}
	void value(T v, unsigned int pos, unsigned int bits)
	{
		unsigned int mask = ((1<<bits) -1) << pos;
		val = (val & (~mask)) | ((std::min(v, T((1<<bits)-1) )  << pos) & mask);
	}

	IntColor(){}

	
	T a()	const {	BOOST_STATIC_ASSERT(n_planes==1 || n_planes==4);	return value(a_pos, a_bits);	}
	void a(T v)	{	BOOST_STATIC_ASSERT(n_planes==1 || n_planes==4);	value(v, a_pos, a_bits);	}

	T r()	const {	BOOST_STATIC_ASSERT(n_planes>=3);	return value(r_pos, r_bits);	}
	T g()	const {	BOOST_STATIC_ASSERT(n_planes>=3);	return value(g_pos, g_bits);	}
	T b()	const {	BOOST_STATIC_ASSERT(n_planes>=3);	return value(b_pos, b_bits);	}
	void r(T v)	{	BOOST_STATIC_ASSERT(n_planes>=3);	value(v, r_pos, r_bits);	}
	void g(T v)	{	BOOST_STATIC_ASSERT(n_planes>=3);	value(v, g_pos, g_bits);	}
	void b(T v)	{	BOOST_STATIC_ASSERT(n_planes>=3);	value(v, b_pos, b_bits);	}

	T u()	const	{	BOOST_STATIC_ASSERT(n_planes==2);	return value(g_pos, g_bits);}
	void u(T v)	{	BOOST_STATIC_ASSERT(n_planes==2);	value(v, g_pos, g_bits);		}
	T v()	const	{	BOOST_STATIC_ASSERT(n_planes==2);	return value(b_pos, b_bits);}
	void v(T v)	{	BOOST_STATIC_ASSERT(n_planes==2);	value(v, b_pos, b_bits);		}

	T l()	const {	BOOST_STATIC_ASSERT(n_planes==1);	return value(a_pos, a_bits);	}
	void l(T v)	{	BOOST_STATIC_ASSERT(n_planes==1);	value(v, a_pos, a_bits);		}

	T plane(unsigned int p) const
	{
		switch(p)
		{
		case 0:	return value(r_pos, r_bits);	break;
		case 1:	return value(g_pos, g_bits);	break;
		case 2:	return value(b_pos, b_bits);	break;
		case 3:	return value(a_pos, a_bits);	break;
		}
		return 0;
	}

	void plane(unsigned int p, T v)
	{
		switch(p)
		{
		case 0:	value(v, r_pos, r_bits);	break;
		case 1:	value(v, g_pos, g_bits);	break;
		case 2:	value(v, b_pos, b_bits);	break;
		case 3:	value(v, a_pos, a_bits);	break;
		}
	}

	
	template<typename T1, unsigned int nPlanes1, unsigned int aBits1, unsigned int aPos1, unsigned int rBits1, unsigned int rPos1, unsigned int gBits1, unsigned int gPos1, unsigned int bBits1, unsigned int bPos1>
	IntColor(const IntColor<T1, nPlanes1, aBits1, aPos1, rBits1, rPos1, gBits1, gPos1, bBits1, bPos1>& c) : val(0)
	{
		value(c.value(aPos1, aBits1), a_pos, a_bits);
		value(c.value(rPos1, rBits1), r_pos, r_bits);
		value(c.value(gPos1, gBits1), g_pos, g_bits);
		value(c.value(bPos1, bBits1), b_pos, b_bits);
	}
};

enum IntColorFormat
{

	ICF_A8 = 1,
	ICF_P8 = 2,


	ICF_R5G6B5 = 3,
	ICF_A1R5G5B5 = 4,
	ICF_A4R4G4B4 = 5,


	ICF_X8R8G8B8 = 6,
	ICF_A8R8G8B8 = 7,
};

typedef IntColor<unsigned short,3, 0,0, 5,11, 6,5, 5,0> t_r5g6b5;
typedef IntColor<unsigned short,4, 1,15, 5,10, 5,5, 5,0> t_a1r5g5b5;
typedef IntColor<unsigned int,3, 0,0, 8,16, 8,8, 8,0> t_x8r8g8b8;

};	

#pragma pack(pop)





// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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