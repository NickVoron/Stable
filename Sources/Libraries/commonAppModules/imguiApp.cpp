#include "imguiApp.h"

namespace AppModules
{
	void ImguiAppBase::release() { imgui::modules::Application::save(); }
	void ImguiAppBase::process() { imgui::modules::Application::process(); }
}