#include "screenLog.h"
#include "uniguidraw/library.include.h"
#include "imgui/library.include.h"
#include "resourceFont/library.include.h"

namespace AppModules
{

ScreenLogBase::ScreenLogBase()
{
	showScreenLog = true;
}

static bool enabledSwitcher = true;
void ScreenLogBase::process()
{
	Input::switcher(Input::KEY_O, enabledSwitcher);

	if (enabledSwitcher && !Viewports::viewports.empty())
	{
		Viewports::Viewport* vh = Viewports::viewports.first();
 		imgui::core::begin(vh);
		vh->set();
 		imgui::vertical_panel_begin(3.0f, 3.0f, 200.0f, (float) vh->height(), false);
 		imgui::checkbox("Show Screen Report", showScreenLog);
 		if (showScreenLog)
 		{
			for (auto& str : logs::report().list())
			{
				imgui::label(str);
			}
 		}
 		imgui::vertical_panel_end();
 		imgui::core::end();
	}	
}


}