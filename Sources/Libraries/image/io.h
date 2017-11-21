#pragma once

#include "color.h"
#include "plane.h"

#include "stream/library.include.h"

namespace image
{
	namespace io
	{
		template<Format fmt>
		stream::ostream& save(stream::ostream& os, const Plane< Color<fmt> >& img)
		{
			os << img.sizeX << img.sizeY;
			os.write(img.bmp, img.memory());
			return os;
		}

		template<Format fmt>
		stream::istream& load(stream::istream& is, Plane< Color<fmt> >& img)
		{
			int sizeX, sizeY;
			is >> sizeX >> sizeY;
			img.allocate(sizeX, sizeY);
			is.read(img.bmp, img.memory());
			return is;
		}
	}
}

template<image::Format fmt> stream::ostream& operator<<(stream::ostream& os, const image::Plane< image::Color<fmt> >& img) { return image::io::save(os, img); }
template<image::Format fmt> stream::istream& operator>>(stream::istream& is, image::Plane< image::Color<fmt> >& img)		{ return image::io::load(is, img); }