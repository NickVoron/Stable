#include "uniguiFlusher.h"

namespace AppModules
{
	void UniguiFlusherBase::process()	
	{ 
		if (gapi::initialized(gapi::DX9))
		{
			
		}
		else if (gapi::initialized(gapi::DX11))
		{
			imgui::core::draw();
			imgui::core::reload_render_target();
		}
	}
}
