// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "application.h"

#include "settingsRegistry/library.include.h"
#include "inputSys/library.include.h"
#ifdef USE_WINDOWS
#include "imgui/library.include.h"
#include "uniguidraw/library.include.h"
#endif


namespace imgui
{
namespace modules
{

Module::Module()
{
	active = true;
	switcher = Input::ERROR_KEYBOARD_CODE;

	Application::add(*this);
}

void Module::release()
{
	
}

const wchar_t* Module::viewportName() const
{
	return L"default";
}

void Module::processInternal()
{

	if(Application::showActivationFlags)
	{
		std::string hotkeyText;
		
		
		
		
        #ifdef USE_WINDOWS
		imgui::checkbox(str::stringize(name(), hotkeyText ).c_str(), active);
        #endif
	}
	
	if(active)
	{
        #ifdef USE_WINDOWS
		imgui::vertical_separator(3.0f);
		process();
		imgui::vertical_separator(15.0f);
        #endif
	}
}




void Modules::process()
{
	if(!empty())
	{
        #ifdef USE_WINDOWS
		struct ScopedViewport
		{
			ScopedViewport(Viewports::Viewport* vp_) : vp(vp_)	{ imgui::vertical_panel_begin((float)vp->width() - 200.0f, 0.0f, (float)vp->width(), (float)vp->height(), false);	}
			~ScopedViewport()									{ imgui::vertical_panel_end(); }
			Viewports::Viewport* vp;
		};

		std::map<Viewports::Viewport*, std::vector<Module*> > sortedViewports;

		for (auto module = begin(); module != end(); ++module)
		{
			Viewports::Viewport* vp = Viewports::viewports.find(module->viewportName());
			if (vp)
			{
				sortedViewports[vp].push_back(&*module);
			}
		}		

 		for(auto& vpi : sortedViewports)
 		{
 			imgui::core::begin(vpi.first);
			vpi.first->set();
 			ScopedViewport scvp(vpi.first);
 			for(auto& module : vpi.second)
 			{
 				module->processInternal();
 			}
 			imgui::core::end();
 		}
        #endif
	}
}

void Modules::release()
{
	for (auto module = begin(); module != end(); ++module)
	{
		module->release();
	}
}

void Modules::save()	
{	
	for (auto& module : *this)
	{	
		SettingsRegistry::add(str::stringize("ImguiAppSettings_", module.name()), module.active);	
	}	
}
void Modules::load()	
{	
	for (auto& module : *this)	
	{	
		std::string name = module.name();
		
		SettingsRegistry::get(str::stringize("ImguiAppSettings_", name), module.active);
	}	
}




bool Application::showActivationFlags = true;
bool Application::hideOnMouseR = true;

Application& Application::get()
{
	static Application app;
	return app;
}

Application::Application()
{
	mem::memzero(switchers);
	enabledSwitcher = true;
}

void Application::add(Module& module)
{
	Application& app = get();

	app.activateSwitcher(module.switcher);
	app.uiModules.push_back(module);
}

void Application::process()
{
	Application& app = get();

	Input::switcher(Input::KEY_I, app.enabledSwitcher);

	if(app.enabledSwitcher)
	{
		if ((!Input::mouseR() && hideOnMouseR) || !hideOnMouseR)
		{
			app.uiModules.process();
		}
	}	
    #ifdef USE_WINDOWS
	Input::inputProcessed( imgui::core::mouseProcessed() );
    #endif
}

void Application::release() 
{ 
 	get().uiModules.release();
 	get().uiModules.remove_all();
}

void Application::save()	
{
	get().uiModules.save();	
}

void Application::load()	
{ 
	get().uiModules.load();	
}

void Application::activateSwitcher(Input::KeyCode& switcher)
{
	if(switcher == Input::ERROR_KEYBOARD_CODE)
	{
		for (int i = Input::KEY_1; i <= Input::KEY_9; ++i)
		{
			if(!switchers[i])
			{
				switcher = (Input::KeyCode)i;
				break;
			}
		}
	}
	else
	{
		ENFORCE(!switchers[switcher]);
	}

	switchers[switcher] = true;
}




Editor::Editor(const char* n, EditFunction func):function(func), caption(n)
{

}

Editor::Editor(EditFunction func):function(func)
{

}

void Editor::process()
{
	function();
}

const char* Editor::name() const
{
	return caption.c_str();
}


}
}




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