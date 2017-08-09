// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

namespace Base{



template< int D > class NDVector
{
public:
	float val[D];

	NDVector<D>()	{}
	NDVector<D>(const NDVector<D>& d1)	{	for(int d=0; d<D; ++d)	val[d] = d1[d];	}
	NDVector<D>(float f)	{	for(int d=0; d<D; ++d)	val[d] = f;	}

	float operator[](int d)	const {	return val[d];	}
	float& operator[](int d)	{	return val[d];	}

	NDVector<D>& operator -= (const NDVector<D>& d1)	{	for(int d=0; d<D; ++d)	val[d] -= d1[d];	return *this;	}
	NDVector<D>& operator += (const NDVector<D>& d1)	{	for(int d=0; d<D; ++d)	val[d] += d1[d];	return *this;	}

	inline int minVal() const;
	inline int maxVal() const;
};



template< int D >
NDVector<D> operator + (const NDVector<D>& d1, const NDVector<D>& d2)
{
	NDVector<D> r;
	for(int d=0; d<D; ++d)	r[d] = d1[d] + d2[d];
	return r;
}

template< int D >
NDVector<D> operator - (const NDVector<D>& d1, const NDVector<D>& d2)
{
	NDVector<D> r;
	for(int d=0; d<D; ++d)	r[d] = d1[d] - d2[d];
	return r;
}

template< int D >
NDVector<D> operator * (float f, const NDVector<D>& d1)
{
	NDVector<D> r;
	for(int d=0; d<D; ++d)	r[d] = f*d1[d];
	return r;
}



template< int D >
int NDVector<D>::minVal() const
{
	float minb = +FLT_MAX;
	int r;
	for(int d=0; d<D; ++d)
		if(val[d] < minb)
		{
			minb = val[d];
			r = d;
		}
	return r;
}


template< int D >
int NDVector<D>::maxVal() const
{
	float maxb = -FLT_MAX;
	int r;
	for(int d=0; d<D; ++d)
		if(val[d] > maxb)
		{
			maxb = val[d];
			r = d;
		}
	return r;
}

};



// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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