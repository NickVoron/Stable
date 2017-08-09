// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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