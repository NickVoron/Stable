// Copyright (C) 2012-2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



// Copyright (C) 2012-2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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