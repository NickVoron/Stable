// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "image.h"
#include "../imgui.h"
#include "../controlTypes.h"
#include "../globals.h"

#include "uniguidraw/library.include.h"

namespace imgui
{

	std::vector<multiapi::Texture*> textures;

	namespace core
	{
		void clear_image_cache()
		{
			textures.clear();
		}
	}
	
	
	void image(const Point2& pos, const StdImage& img)
	{
		image(pos, Point2(img.sizeX, img.sizeY), img);
	}

	void image(const Point2& pos, const Point2& size, const StdImage& img)
	{
		if (!img.empty())
		{
			textures.emplace_back(new multiapi::Texture());
 			auto texture = textures.back();
 			texture->create(img);

			unigui::Rect rect(pos.x, pos.y, size.x, -size.y);
			unigui::draw::texrect(rect, texture);
			
		}
	}

	void image(const StdImage& img)
	{
		image(Point2(0,0), img);
	}

	void image(const Vector3& pos, const StdImage& img)
	{
		Point2 p;
		if (detail::worldToScreen(pos, p))
		{
			image(p, img);
		}
	}

	void image(const Vector3& pos, float worldWidth, const StdImage& img)
	{
		Point2 p, c;
		if (detail::worldToScreen(pos, p))
		{
			Vector2 size(worldWidth, ((float)img.sizeY / img.sizeX) * worldWidth);
			Vector3 corner = detail::billboardPosition(pos, size);
			detail::worldToScreen(corner, c);
			image(p, c - p, img);
		}
	}

}//



// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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