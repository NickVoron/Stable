#include "imgui.h"
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
		detail::font = Resources::Font::add("Lora-Regular.ttf", 11);
		//detail::font = Resources::Font::add("Inconsolata.otf", 16);
		detail::guiAtlas = Resources::Atlas::add("gui.atlas");

		renderTargets = std::make_unique<RenderTargets>();
		renderTargets->init();
		renderTargets->initTarget(Viewports::viewports.first());
		
 
//  		unigui::font::FontExport fontExport;
//  
//  		fontExport.addRange(0x0000, 0x00FF);
//  		fontExport.addUTF16(L"О каждом из нас заботится товарищ Сталин в Кремле!");
//  		fontExport.compile(detail::face, "testFont", "Arial.ttf", 14);
//  
//  		detail::font.init(detail::face);
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

		//draw();
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
