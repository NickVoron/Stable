#include "application.h"
#include "uniguidraw/library.include.h"
#include "settingsRegistry/library.include.h"
#include "InputLib/library.include.h"
#include "imgui/library.include.h"


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
	//switchAction = 0;
}

const wchar_t* Module::viewportName() const
{
	return L"default";
}

void Module::processInternal()
{
/*
	if(switcher != Input::ERROR_KEYBOARD_CODE)
	{
		//switchAction.init() = Input::action(str::stringize("INGUI_MODULE_ACTIVATOR_", name(), switcher).c_str(), switcher);
		switchAction.init(switcher);
	}

	if(switchAction.pressed(true))
	{
		active = !active;
	}
*/
	if(Application::showActivationFlags)
	{
		std::string hotkeyText;
		// 	if(switchAction)
		// 	{
		// 		hotkeyText = str::stringize(" (",  switchAction->GetTextDesc(), ")").str();
		// 	}

		imgui::checkbox(str::stringize(name(), hotkeyText ).c_str(), active);
	}
	
	if(active)
	{
		imgui::vertical_separator(3.0f);
		process();
		imgui::vertical_separator(15.0f);
	}
}

//
//
//
void Modules::process()
{
	if(!empty())
	{
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
		//LOG_MSG(name);
		SettingsRegistry::get(str::stringize("ImguiAppSettings_", name), module.active);
	}	
}

//
//
//
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

	Input::inputProcessed( imgui::core::mouseProcessed() );
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

//
//
//
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