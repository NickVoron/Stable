#pragma once

#include "imgui.h"
#include "settings.h"
#include "debugDrawing.h"
#include "window.h"

#include "imguiModules/library.include.h"


namespace AppModules
{
	class ImguiAppBase
	{
	public:
		void release();
		void process();
	};

	template<class Prologue>
	class ImguiAppBaseT : public ImguiAppBase, public ModuleBase<ImguiAppBaseT<Prologue>, Prologue>
	{
	public:
		template<class Params>  void release(Params& p)	{ ImguiAppBase::release();	}
		template<class Params>  void process(Params& p)	{ ImguiAppBase::process();	}

		template<class WindowType>
		void create(WindowsGUI::Windows<WindowType>& windows)
		{
			imgui::modules::Application::load();
		}
	};

	struct ImguiApp9	: public ImguiAppBaseT<Loki::MakeTypelist<Window, Imgui9, Settings, DebugDrawingModule9>> { };
	struct ImguiApp11	: public ImguiAppBaseT<Loki::MakeTypelist<Window, Imgui11, Settings, DebugDrawingModule11>>
	{
		template<class Params>  void init(Params& p)		{	create(p.windows);	}	
	};
}