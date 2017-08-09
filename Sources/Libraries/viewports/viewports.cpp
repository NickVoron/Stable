// Copyright (C) 2013-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "viewports.h"

#include "defaultLogs/library.include.h"

namespace Viewports
{
	
	
	
	Viewports viewports;

	ViewportHandles::~ViewportHandles()
	{
 		remove_all();
	}

	Viewport::Viewport()
	{
		viewports.push_back(*this);
		camera.defaults();
	}
	
	Viewport::~Viewport()
	{
		remove();		
		camera.remove();
	}

	void Viewport::init(const std::wstring& n, int w, int h)
	{
		name = n;

		if (w > 0 && h > 0)
		{
			camera.setAspect((float) w / h);
		}		
	}

	CalculatedCamera Viewport::calculateCamera() const
	{
		CalculatedCamera result;
		result.calculate(camera.camera().params);
		result.viewportSize.x = width();
		result.viewportSize.y = height();
		return result;
	}

	
	
	
	ViewportHandle::ViewportHandle() 
	{
		bind(0); 
	}
	
	ViewportHandle::ViewportHandle(Viewport* v) 
	{ 
		bind(v); 
	}






	ViewportHandle::~ViewportHandle()
	{
		remove();
	}

	Viewport* ViewportHandle::viewport() 
	{ 

		return isInList() && vp ? vp : 0; 
	}

	void ViewportHandle::bind(Viewport* v) 
	{ 
		vp = v; 
		if (v)
		{
			remove();
			v->push_back(*this);
		}
	}

	
	
	Viewport* Viewports::find(const std::wstring& name)
	{
		for (auto& v : *this)
		{
			if(v.name == name)
			{
				return &v;
			}
		}

		return 0;
	}

	bool equal(const Viewport* f, const Viewport* s)
	{
		return f && s && f == s && f->width() == s->width() && f->height() == s->height() && f->deviceIndex() == s->deviceIndex();
	}

}



// Copyright (C) 2013-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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