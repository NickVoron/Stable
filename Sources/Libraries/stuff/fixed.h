// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once



#pragma pack(push,1)

namespace Base
{








template<typename T, int n> 
struct fixed
{
	enum { num_bits = sizeof(T) * 8, point_pos = n };

	T val;

	fixed()	{}
	explicit fixed(T v)	{ val = v << (num_bits-point_pos); }
	explicit fixed(float v)	{ val = T(v*(1<<(num_bits-point_pos))); }

	
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

};	

#pragma pack(pop)




// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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