#pragma once

#include "NDVector.h"

namespace Base{

template< int D > class NDCube
{
public:
	NDVector<D> corner;
	float size;

	NDCube(){}
	NDCube(const NDVector<D>& c, float s) : corner(c), size(s){}

	template<class T> bool isInside(const T& p) const
	{
		for(int d=0; d<D; ++d)
			if( p[d] < corner[d] || p[d] >= corner[d]+size)	return false;
		return true;
	}

	template<class T> bool isIntersect(const T& p, float r) const
	{
		for(int d=0; d<D; ++d)
			if( p[d]+r < corner[d] || p[d]-r >= corner[d]+size)	return false;
		return true;
	}
};

};
