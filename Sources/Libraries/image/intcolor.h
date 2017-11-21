#pragma once

#include <boost/static_assert.hpp>
#include <algorithm>

#pragma pack(push,1)

namespace image
{


/*--------------------------------------------------------------------------
This is a template color class with different color planes organization
supports only load-store instructions with assign constructor for converting
between different types.

There no arithmetic operations - use argbColor instead that support wide 
range and enhanced precision in arithmetic operations.

Typical operations on bitmaps:
1) one bitmap is defined as unsigned short pixels of A1R5G5B5 format
2) other bitmap is defined as unsigned short pixels of R5G6B5 format
3) output bitmap defined as A8R8G8B8

const IntColor<ushort,4, 1,15, 5,10, 5,5, 5,0>& srcColor0;
const IntColor<ushort,3, 0,0, 5,11, 6,5, 5,0>& srcColor1;
IntColor<unsigned int,4, 8,24, 8,16, 8,8, 8,0>& destColor;

4) operation will be performed as multiplying

argbColor<ushort> s0(srcColor0);
argbColor<uchar> s1(srcColor1);
argbColor<ushort> res = s0*s1;	//here automatic deducting of result type is performed argbColor<ushort>(s0*s1)

destColor = res; //converting to the destination format

--------------------------------------------------------------------------*/
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

	//different methods for various number of color planes
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

	//converting from one type to other
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
//8 bpp
	ICF_A8 = 1,
	ICF_P8 = 2,

//16 bpp
	ICF_R5G6B5 = 3,
	ICF_A1R5G5B5 = 4,
	ICF_A4R4G4B4 = 5,

//32 bpp
	ICF_X8R8G8B8 = 6,
	ICF_A8R8G8B8 = 7,
};

typedef IntColor<unsigned short,3, 0,0, 5,11, 6,5, 5,0> t_r5g6b5;
typedef IntColor<unsigned short,4, 1,15, 5,10, 5,5, 5,0> t_a1r5g5b5;
typedef IntColor<unsigned int,3, 0,0, 8,16, 8,8, 8,0> t_x8r8g8b8;

};	//art

#pragma pack(pop)

