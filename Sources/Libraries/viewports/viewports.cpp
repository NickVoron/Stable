#include "viewports.h"

#include "defaultLogs/library.include.h"

namespace Viewports
{
	//
	//
	//
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

	//
	//
	//
	ViewportHandle::ViewportHandle() 
	{
		bind(0); 
	}
	
	ViewportHandle::ViewportHandle(Viewport* v) 
	{ 
		bind(v); 
	}

// 	ViewportHandle::ViewportHandle(const ViewportHandle& vh)
// 	{ 
// 		bind(vh.vp); 
// 	}

	ViewportHandle::~ViewportHandle()
	{
		remove();
	}

	Viewport* ViewportHandle::viewport() 
	{ 
//		return vp;
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

	//
	//
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