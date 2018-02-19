// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "InputSystem.h"
#include <math.h>

#include "commands.h"

namespace Input
{

std::string InputEventToStr(InputEvent ev);
std::string InputModifierToStr(int modifiers);

InputSystem* InputSystem::current = nullptr;

InputSystem* InputSystem::get()	
{	
	return current;	
}

InputSystem::InputSystem()
{
	enabledValue = true;
}

bool InputSystem::enabled() const
{
	return enabledValue;
}

void InputSystem::enabled(bool e)
{
	enabledValue = e;

}

float InputSystem::keyboard(KeyCode code)
{
	float p;
	return keyboard(code, p);
}

float InputSystem::mouse(MouseCode code)
{
	float p;
	return mouse(code, p);
}

float InputSystem::directpad2(DirectInputCode2 code)
{
	float p;
	return directpad2(code, p);
}

float InputSystem::xpad(unsigned int index, XPadCode code)
{
	float p;
	return xpad(index, code, p);
}

float InputSystem::keyboard(KeyCode code, float& prev)
{
	return enabledValue ? keyboard_impl(code, prev) : (prev = 0.0f);
}

float InputSystem::mouse(MouseCode code, float& prev)
{
	
	return enabledValue ? mouse_impl(code, prev) : (prev = 0.0f);
}

float InputSystem::directpad2(DirectInputCode2 code, float& prev)
{
	return enabledValue ? directpad2_impl(code, prev) : (prev = 0.0f);
}

float InputSystem::xpad(unsigned int index, XPadCode code, float& prev)
{
	return enabledValue ? xpad_impl(index, code, prev) : (prev = 0.0f);
}

InputEvent::InputEvent() 
{ 
	data = 0; 
}

InputEvent::InputEvent(KeyCode c) 
{ 
	data = c | KEYBOARD; 
}

InputEvent::InputEvent(MouseCode c) 
{
	data = c | MOUSE; 
}

InputEvent::InputEvent(DirectInputCode2 c)
{
	data = c | DIPAD; 
}

InputEvent::InputEvent(XPadCode c, int index) 
{ 
	data = c | XPAD; 
	data |= index << 30;
}

InputEvent::DEVICE_TYPE_FLAGS InputEvent::device() const
{
	return (DEVICE_TYPE_FLAGS)(data & TYPE_MASK);
}

int	InputEvent::code() const
{
	return data & CODE_MASK; 
}

int InputEvent::port() const
{
	return (data & PORT_MASK) >> 30; 
}


float InputEvent::value(float& prev) const
{
	InputSystem* is = InputSystem::get();
	switch(device())
	{
	case InputEvent::KEYBOARD:	return is->keyboard((KeyCode)code(), prev);
	case InputEvent::MOUSE:		return is->mouse((MouseCode)code(), prev);
	case InputEvent::XPAD:		return is->xpad(port(), (XPadCode)code(), prev);
	case InputEvent::DIPAD:		return is->directpad2((DirectInputCode2)code(), prev);
	};

	return 0.0f;
}

float InputEvent::value() const
{
	float prev;
	return value(prev);
}





void Action::init(const InputEvent& ev, int mod)
{
	event = ev;
	modifier = mod;
}

void Action::report() const
{
	return;

	if (modifier == MOD_NONE)
	{
		LOG_REPORT(InputEventToStr(event));
	}
	else
	{
		LOG_REPORT(InputModifierToStr(modifier) << "+" << InputEventToStr(event));
	}
}

bool Action::state(bool press, bool now) const
{
	report();

	if(!checkModifier(modifier))
		return false;

	float prev = 0.0f;
	float value = event.value(prev);
	
	bool changed = (value != prev);
	bool res = (value != (press ? 0.0f : 1.0f));
	if(now && res)
	{
		res = changed;
	}

	return res;
}

bool Action::pressed(bool now) const
{
	return state(true, now);	
}

bool Action::released(bool now) const
{
	return state(false, now);
}

float Action::value() const
{
	report();
	return !checkModifier(modifier) ? 0.0f : event.value();
}

float Action::delta() const
{
	report();
	if(!checkModifier(modifier))
		return 0.0f;

	float prev = 0.0f;
	float value = event.value(prev);

	return value - prev;
}






bool checkModifier(int mod)
{
	
	if(mod == MOD_NONE)
		return true; 

	static int keyarray[] =
	{
		KEY_LCONTROL,
		KEY_RCONTROL,
		KEY_LMENU,
		KEY_RMENU,
		KEY_LWIN,
		KEY_RWIN,
		KEY_LSHIFT,
		KEY_RSHIFT,
		KEY_CAPITAL,
		MOUSE_BUTTON_LEFT,
		MOUSE_BUTTON_RIGHT, 
		MOUSE_BUTTON_MIDDLE,
	};


	int m = mod;
	
	for(int i = 0; i < (sizeof(keyarray) / sizeof(int)); ++i)
	{
		
		int bit = 1 << i;
		if(m & bit)
		{
			
			if( (i < MOD_KEY_COUNT) ? keyboard((KeyCode)keyarray[i]) : mouse((MouseCode)keyarray[i]) )
			{
				
				m &= ~bit;
			}
			else
			{
				
				return false;
			}
		}
	}

	return m == 0; 
}

float mouseX()	{ return InputSystem::get()->mouse(MOUSE_AXIS_X); }
float mouseY()	{ return InputSystem::get()->mouse(MOUSE_AXIS_Y); }
float mouseZ()	{ return InputSystem::get()->mouse(MOUSE_AXIS_Z); }

bool mouseL(bool now) { return mouse(MOUSE_BUTTON_LEFT, now); }
bool mouseM(bool now) { return mouse(MOUSE_BUTTON_MIDDLE, now); }
bool mouseR(bool now) { return mouse(MOUSE_BUTTON_RIGHT, now); }
bool mouseX1(bool now) { return mouse(MOUSE_BUTTON_X1, now); }
bool mouseX2(bool now) { return mouse(MOUSE_BUTTON_X2, now); }
bool mouseX3(bool now) { return mouse(MOUSE_BUTTON_X3, now); }
bool mouseX4(bool now) { return mouse(MOUSE_BUTTON_X4, now); }
bool mouseX5(bool now) { return mouse(MOUSE_BUTTON_X5, now); }


bool keyboardCTRL(bool now)	{ return keyboard(KEY_CONTROL, now); }
bool keyboardSHIFT(bool now){ return keyboard(KEY_SHIFT, now); }

bool xpadA(int index, bool now) { return xpad(index, XPAD_BUTTON_A, now); }
bool xpadB(int index, bool now) { return xpad(index, XPAD_BUTTON_B, now); }
bool xpadX(int index, bool now) { return xpad(index, XPAD_BUTTON_X, now); }
bool xpadY(int index, bool now) { return xpad(index, XPAD_BUTTON_Y, now); }

bool keyboard(KeyCode code, bool now)			{ return universal(0, code, now); }
bool mouse(MouseCode code, bool now)			{ return universal(0, code, now); }
bool directpad2(DirectInputCode2 code, bool now){ return universal(0, code, now); };
bool xpad(int index, XPadCode code, bool now)	{ return universal(index, code, now); }

template<> bool universal(int deviceIndex, KeyCode code, float& prev, float& current)			{ auto input = InputSystem::get(); if (input) current = input->keyboard(code, prev);  return input != nullptr; }
template<> bool universal(int deviceIndex, MouseCode code, float& prev, float& current)			{ auto input = InputSystem::get(); if (input) current = input->mouse(code, prev); return input != nullptr; }
template<> bool universal(int deviceIndex, XPadCode code, float& prev, float& current)			{ auto input = InputSystem::get(); if (input) current = input->xpad(deviceIndex, code, prev); return input != nullptr; }
template<> bool universal(int deviceIndex, DirectInputCode2 code, float& prev, float& current)	{ auto input = InputSystem::get(); if (input) current = input->directpad2(code, prev); return input != nullptr; }

void inputProcessed(bool processed)
{
	InputSystem::get()->enabled(!processed);
}



Action action(int index, XPadCode code, int modifiers)
{
	Action res; res.init(InputEvent(code, index), modifiers);
	return res;
}

}



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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