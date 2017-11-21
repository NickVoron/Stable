#pragma once

namespace Base{

//N-Dimensional Vector

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