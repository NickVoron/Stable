#pragma once

//#include "../config.h"

#pragma pack(push,1)

namespace Base
{

//fpi - fixed-point int
//fps - fixed-point short
//fpc - fixed-point char
//fpui - fixed-point unsigned int
//fpus - fixed-point unsigned short
//fpuc - fixed-point unsigned char

template<typename T, int n> 
struct fixed
{
	enum { num_bits = sizeof(T) * 8, point_pos = n };

	T val;

	fixed()	{}
	explicit fixed(T v)	{ val = v << (num_bits-point_pos); }
	explicit fixed(float v)	{ val = T(v*(1<<(num_bits-point_pos))); }

	//assign operator with conversion position of fixed point
	template<int nn> 
	fixed(const fixed<T, nn>& i)
	{
		int pd = n - nn;
		val = (pd<0) ? (i.val << (-pd)) ? (i.val >> (pd));
	}

	operator T() { return T(val) >> (num_bits-point_pos); }
	operator float() { return float(val) / (1 << (num_bits-point_pos)); }	
		
	template<int nn> 
	void operator += (const fixed<T, nn>& v)
	{
		int pd = point_pos - nn;
		val += (pd<0) ?	(v.val << (-pd)) : (v.val >> (pd));
	}

	template<int nn> 
	void operator *=(const fixed<T, nn>& v)
	{
		int rshift = 16 - nn;
		val = ( ((boost::intmax_t)val) * v.val) >> rshift;
	}
};

//metaprogramming template for constructing int value to maximize range of fixed-point value as result of addition, multiplication and substraction
template<int n1, int n2> struct MaxRangeMul { enum { val = n1 + n2 }; };
template<int n1, int n2> struct MaxRangeAdd { enum { val = n1 > n2 ? n1 : n2 }; };
template<int n1, int n2> struct MaxRangeSub { enum { val = n1 > n2 ? n1 : n2 }; };

template<typename T1, typename T2> 
fixed<MaxRangeMul< T1::point_pos, T2::point_pos >::val> operator*(T1 t1, T2 t2)
{
	fixed<MaxRangeMul<T1::point_pos, T2::point_pos>::val> r;
	boost::intmax_t m = ((unsigned int)t1.val)*t2.val;
	r.assign((unsigned short)(m>>16));
	return r;
};

template<class T1, class T2> fpus< MaxRangeAdd< T1::point_pos, T2::point_pos >::val > operator + (T1 t1, T2 t2)
{
	int pd = T2::point_pos - T1::point_pos;

	fpus< MaxRangeAdd< T1::point_pos, T2::point_pos >::val > v;
	if(pd<0)	v.assign( (t2.val >> (-pd)) + t1.val );
	else	v.assign ( (t1.val >> pd) + t2.val );
	return v;
};

template<class T1, class T2> fpus< MaxRangeSub< T1::point_pos, T2::point_pos >::val > operator - (T1 t1, T2 t2)
{
	int pd = T2::point_pos - T1::point_pos;

	fpus< MaxRangeAdd< T1::point_pos, T2::point_pos >::val > v;
	if(pd<0)	v.assign( (t2.val >> (-pd)) - t1.val );
	else	v.assign ( (t1.val >> pd) - t2.val );
	return v;
};

template<int n, int prec, class T1, class T2> fpus<n> div(T1 t1, T2 t2)
{
	//static int lookup[2^prec];
	unsigned int nom = t1.val;
	unsigned int den = t2.val;

	int pd = T2::point_pos - T1::point_pos;
	if(pd<0)	den >>= (-pd);
	else	nom >>= pd;

	unsigned int res = nom / den;
	res <<= (16-n);

	unsigned int rem = (nom % den) << (16-n);

	res |= rem / den;

	fpus<n> r;
	r.assign(unsigned short( res ) );

	return r;
};

};	//art

#pragma pack(pop)
