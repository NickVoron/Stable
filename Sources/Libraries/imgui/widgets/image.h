#pragma once

#include "../types.h"

#include "image/library.include.h"
#include "imageD3D/library.include.h"


namespace imgui
{
	namespace core
	{
		void clear_image_cache();
	}

	typedef ::image::Plane< ::image::Color<::image::RGBA8> > StdImage;

	void image(const Vector3& pos, const StdImage& img);
	void image(const Vector3& pos, float worldWidth, const StdImage& img);
	void image(const Point2& pos, const StdImage& img);
	void image(const Point2& pos, const Point2& size, const StdImage& img);
	void image(const StdImage& img);
	
	template<image::Format format>
	void image(const ::image::Plane< ::image::Color<format> >& img)
	{
		if(!img.empty())
		{
 			StdImage simg;
 			simg.copy(img);
			image(simg);
		}		
	}

	template<class PositionType, image::Format format>
	void image(const PositionType& pos, const ::image::Plane< ::image::Color<format> >& img)
	{
		if (!img.empty())
		{
			StdImage simg;
			simg.copy(img);
			image(pos, simg);
		}
	}

	template<image::Format format>
	void image(const Vector3& pos, float worldWidth, const ::image::Plane< ::image::Color<format> >& img)
	{
		if (!img.empty())
		{
			StdImage simg;
			simg.copy(img);
			image(pos, worldWidth, simg);
		}
	}
}