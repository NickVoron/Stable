#pragma once

#include "plane.h"
namespace image
{

template<class ColorT> 
class PlaneMips
{
	typedef Plane<ColorT> Image;

	std::vector< Image > mips;
public:

	PlaneMips(size_t n)
	{
		mips.resize(n);
	}

	void assign(size_t m, const Plane<ColorT>& im)
	{
		mips[m] = im;
	}

	Plane<ColorT>& operator [](size_t m)
	{
		return mips[m];
	}
	
	size_t numMips() const
	{
		return mips.size();
	}

	void generateMips()
	{
		for(size_t m=1; m < mips.size(); ++m)
		{
			Plane<ColorT>& src = mips[m-1];
			Plane<ColorT>& dest = mips[m];

			for(int y=0; y < dest.sizeY; ++y)
			{
				for(int x=0; x < dest.sizeX; ++x)
				{
					ColorT c0 = src(x*2 + 0, y*2 + 0);
					ColorT c1 = src(x*2 + 0, y*2 + 1);
					ColorT c2 = src(x*2 + 1, y*2 + 0);
					ColorT c3 = src(x*2 + 1, y*2 + 1);

					ColorT c = (c0 + c1 + c2 + c3)*0.25f;
					dest(x, y) = c;
				}
			}
		}
	}

};

};	//art
