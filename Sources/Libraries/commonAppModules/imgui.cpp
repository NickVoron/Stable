#include "imgui.h"

namespace AppModules
{
	void ImguiBase::init()		
	{
 		imgui::core::init();
		imgui::core::reload_render_target();
	};

	void ImguiBase::process(float dt)	
	{


		imgui::Mouse mouse;
		imgui::Keyboard keyboard;

		mem::memzero(mouse);

		if (Input::action(Input::MOUSE_BUTTON_LEFT).pressed(true))			{ mouse.lmb = imgui::Mouse::NOW_DOWN;	}
 		else if(Input::action(Input::MOUSE_BUTTON_LEFT).pressed(false))		{ mouse.lmb = imgui::Mouse::DOWN;		}
 		else if(Input::action(Input::MOUSE_BUTTON_LEFT).released(true))		{ mouse.lmb = imgui::Mouse::NOW_UP;		}
 		else																{ mouse.lmb = imgui::Mouse::UP;			}
 
 		mouse.pos.x = Input::mouseX();
 		mouse.pos.y = Input::mouseY();

		static float wheelValueOld = Input::mouseZ();
		float wheelValue = Input::mouseZ();
 		mouse.wheelDelta = wheelValueOld - wheelValue;
		wheelValueOld = wheelValue;
 		imgui::core::process(mouse, keyboard, dt);
	}

	void ImguiBase::release()	
	{
		imgui::core::destroy();
	}
}