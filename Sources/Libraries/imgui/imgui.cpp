// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "imgui.h"
#ifdef USE_WINDOWS
#include "globals.h"
#include "renderTarget.h"

#include "resourceFont/library.include.h"
#include "resourceAtlas/library.include.h"

namespace imgui 
{
namespace core 
{
	std::unique_ptr<RenderTargets> renderTargets;

	Viewports::ViewportHandle currentViewportHandler;

	void init()
	{
		
		
		detail::guiAtlas = Resources::Atlas::add("gui.atlas");

		renderTargets = std::make_unique<RenderTargets>();
		renderTargets->init();
		renderTargets->initTarget(Viewports::viewports.first());
		
 







		unigui::draw::init();

	}

	void reload_render_target()
	{
		if (!Viewports::viewports.empty())
		{
			renderTargets->clear(Viewports::viewports.first());
		}		
	}

	void draw()
	{
		if (!Viewports::viewports.empty())
		{
			renderTargets->draw(Viewports::viewports.first());
		}		
	}

	

	void begin(Viewports::Viewport* viewport)
	{
		renderTargets->bind(Viewports::viewports.first());
		unigui::draw::begin(viewport);
	}

	void end()
	{
 		if (currentViewportHandler.viewport())
 		{
 			unigui::draw::end();
			currentViewportHandler.remove();
 		}		
	}

	void checked_begin()
	{
 		if (!currentViewportHandler.viewport())
 		{
 			currentViewportHandler.bind(Viewports::viewports.first());
 		}

 		if (currentViewportHandler.viewport())
 		{
 			begin(currentViewportHandler.viewport());
 		}
	}

	void checked_end()
	{
		end();
	}

	void process(const Mouse& m, const Keyboard& k, float dt)
	{
		if(detail::mouse.lmb == Mouse::NOW_UP || detail::mouse.lmb == Mouse::UP)
		{
			capture::reset();
		}

 		detail::mouse = m;
 		detail::keyboard = k;

		
	}

	bool mouseProcessed()
	{
		return detail::mouse.processed();
	}

	void destroy()
	{
		unigui::draw::release();		
	}
}	
}
#endif




// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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