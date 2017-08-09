// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#ifdef WIN32

#include "inputSysWin.h"
#include "directInput.h"
#include <windows.h>

#pragma comment(lib, "xinput.lib")
#pragma comment(lib, "dinput8.lib")

namespace Input
{

WinInput::KeyboardDict::KeyboardDict()
{ 
	data[0][VK_LBUTTON]				=	KEY_LBUTTON;
	data[0][VK_RBUTTON]				=	KEY_RBUTTON;
	data[0][VK_CANCEL]				=	KEY_CANCEL;
	data[0][VK_MBUTTON]				=	KEY_MBUTTON;
	data[0][VK_XBUTTON1]			=	KEY_XBUTTON1;
	data[0][VK_XBUTTON2]			=	KEY_XBUTTON2;
	data[0][VK_BACK]				=	KEY_BACK;
	data[0][VK_TAB]					=	KEY_TAB;
	data[0][VK_CLEAR]				=	KEY_CLEAR;
	data[0][VK_RETURN]				=	KEY_RETURN;
	data[0][VK_SHIFT]				=	KEY_SHIFT;
	data[0][VK_CONTROL]				=	KEY_CONTROL;
	data[0][VK_MENU]				=	KEY_MENU;
	data[0][VK_PAUSE]				=	KEY_PAUSE;
	data[0][VK_CAPITAL]				=	KEY_CAPITAL;
	data[0][VK_KANA]				=	KEY_KANA;
	data[0][VK_HANGEUL]				=	KEY_HANGEUL;
	data[0][VK_HANGUL]				=	KEY_HANGUL;
	data[0][VK_JUNJA]				=	KEY_JUNJA;
	data[0][VK_FINAL]				=	KEY_FINAL;
	data[0][VK_HANJA]				=	KEY_HANJA;
	data[0][VK_KANJI]				=	KEY_KANJI;
	data[0][VK_ESCAPE]				=	KEY_ESCAPE;
	data[0][VK_CONVERT]				=	KEY_CONVERT;
	data[0][VK_NONCONVERT]			=	KEY_NONCONVERT;
	data[0][VK_ACCEPT]				=	KEY_ACCEPT;
	data[0][VK_MODECHANGE]			=	KEY_MODECHANGE;
	data[0][VK_SPACE]				=	KEY_SPACE;
	data[0][VK_PRIOR]				=	KEY_PRIOR;
	data[0][VK_NEXT]				=	KEY_NEXT;
	data[0][VK_END]					=	KEY_END;
	data[0][VK_HOME]				=	KEY_HOME;
	data[0][VK_LEFT]				=	KEY_LEFT;
	data[0][VK_UP]					=	KEY_UP;
	data[0][VK_RIGHT]				=	KEY_RIGHT;
	data[0][VK_DOWN]				=	KEY_DOWN;
	data[0][VK_SELECT]				=	KEY_SELECT;
	data[0][VK_PRINT]				=	KEY_PRINT;
	data[0][VK_EXECUTE]				=	KEY_EXECUTE;
	data[0][VK_SNAPSHOT]			=	KEY_SNAPSHOT;
	data[0][VK_INSERT]				=	KEY_INSERT;
	data[0][VK_DELETE]				=	KEY_DELETE;
	data[0][VK_HELP]				=	KEY_HELP;
	data[0][VK_LWIN]				=	KEY_LWIN;
	data[0][VK_RWIN]				=	KEY_RWIN;
	data[0][VK_APPS]				=	KEY_APPS;
	data[0][VK_SLEEP]				=	KEY_SLEEP;
	data[0][VK_NUMPAD0]				=	KEY_NUMPAD0;
	data[0][VK_NUMPAD1]				=	KEY_NUMPAD1;
	data[0][VK_NUMPAD2]				=	KEY_NUMPAD2;
	data[0][VK_NUMPAD3]				=	KEY_NUMPAD3;
	data[0][VK_NUMPAD4]				=	KEY_NUMPAD4;
	data[0][VK_NUMPAD5]				=	KEY_NUMPAD5;
	data[0][VK_NUMPAD6]				=	KEY_NUMPAD6;
	data[0][VK_NUMPAD7]				=	KEY_NUMPAD7;
	data[0][VK_NUMPAD8]				=	KEY_NUMPAD8;
	data[0][VK_NUMPAD9]				=	KEY_NUMPAD9;
	data[0][VK_MULTIPLY]			=	KEY_MULTIPLY;
	data[0][VK_ADD]					=	KEY_ADD;
	data[0][VK_SEPARATOR]			=	KEY_SEPARATOR;
	data[0][VK_SUBTRACT]			=	KEY_SUBTRACT;
	data[0][VK_DECIMAL]				=	KEY_DECIMAL;
	data[0][VK_DIVIDE]				=	KEY_DIVIDE;
	data[0][VK_F1]					=	KEY_F1;
	data[0][VK_F2]					=	KEY_F2;
	data[0][VK_F3]					=	KEY_F3;
	data[0][VK_F4]					=	KEY_F4;
	data[0][VK_F5]					=	KEY_F5;
	data[0][VK_F6]					=	KEY_F6;
	data[0][VK_F7]					=	KEY_F7;
	data[0][VK_F8]					=	KEY_F8;
	data[0][VK_F9]					=	KEY_F9;
	data[0][VK_F10]					=	KEY_F10;
	data[0][VK_F11]					=	KEY_F11;
	data[0][VK_F12]					=	KEY_F12;
	data[0][VK_F13]					=	KEY_F13;
	data[0][VK_F14]					=	KEY_F14;
	data[0][VK_F15]					=	KEY_F15;
	data[0][VK_F16]					=	KEY_F16;
	data[0][VK_F17]					=	KEY_F17;
	data[0][VK_F18]					=	KEY_F18;
	data[0][VK_F19]					=	KEY_F19;
	data[0][VK_F20]					=	KEY_F20;
	data[0][VK_F21]					=	KEY_F21;
	data[0][VK_F22]					=	KEY_F22;
	data[0][VK_F23]					=	KEY_F23;
	data[0][VK_F24]					=	KEY_F24;
	data[0][VK_NUMLOCK]				=	KEY_NUMLOCK;
	data[0][VK_SCROLL]				=	KEY_SCROLL;
	data[0][VK_OEM_NEC_EQUAL]		=	KEY_OEM_NEC_EQUAL;
	data[0][VK_OEM_FJ_JISHO]		=	KEY_OEM_FJ_JISHO;
	data[0][VK_OEM_FJ_MASSHOU]		=	KEY_OEM_FJ_MASSHOU;
	data[0][VK_OEM_FJ_TOUROKU]		=	KEY_OEM_FJ_TOUROKU;
	data[0][VK_OEM_FJ_LOYA]			=	KEY_OEM_FJ_LOYA;
	data[0][VK_OEM_FJ_ROYA]			=	KEY_OEM_FJ_ROYA;
	data[0][VK_LSHIFT]				=	KEY_LSHIFT;
	data[0][VK_RSHIFT]				=	KEY_RSHIFT;
	data[0][VK_LCONTROL]			=	KEY_LCONTROL;
	data[0][VK_RCONTROL]			=	KEY_RCONTROL;
	data[0][VK_LMENU]				=	KEY_LMENU;
	data[0][VK_RMENU]				=	KEY_RMENU;
	data[0][VK_BROWSER_BACK]		=	KEY_BROWSER_BACK;
	data[0][VK_BROWSER_FORWARD]		=	KEY_BROWSER_FORWARD;
	data[0][VK_BROWSER_REFRESH]		=	KEY_BROWSER_REFRESH;
	data[0][VK_BROWSER_STOP]		=	KEY_BROWSER_STOP;
	data[0][VK_BROWSER_SEARCH]		=	KEY_BROWSER_SEARCH;
	data[0][VK_BROWSER_FAVORITES]	=	KEY_BROWSER_FAVORITES;
	data[0][VK_BROWSER_HOME]		=	KEY_BROWSER_HOME;
	data[0][VK_VOLUME_MUTE]			=	KEY_VOLUME_MUTE;
	data[0][VK_VOLUME_DOWN]			=	KEY_VOLUME_DOWN;
	data[0][VK_VOLUME_UP]			=	KEY_VOLUME_UP;
	data[0][VK_MEDIA_NEXT_TRACK]	=	KEY_MEDIA_NEXT_TRACK;
	data[0][VK_MEDIA_PREV_TRACK]	=	KEY_MEDIA_PREV_TRACK;
	data[0][VK_MEDIA_STOP]			=	KEY_MEDIA_STOP;
	data[0][VK_MEDIA_PLAY_PAUSE]	=	KEY_MEDIA_PLAY_PAUSE;
	data[0][VK_LAUNCH_MAIL]			=	KEY_LAUNCH_MAIL;
	data[0][VK_LAUNCH_MEDIA_SELECT]	=	KEY_LAUNCH_MEDIA_SELECT;
	data[0][VK_LAUNCH_APP1]			=	KEY_LAUNCH_APP1;
	data[0][VK_LAUNCH_APP2]			=	KEY_LAUNCH_APP2;
	data[0][VK_OEM_1]				=	KEY_OEM_1;
	data[0][VK_OEM_PLUS]			=	KEY_OEM_PLUS;
	data[0][VK_OEM_COMMA]			=	KEY_OEM_COMMA;
	data[0][VK_OEM_MINUS]			=	KEY_OEM_MINUS;
	data[0][VK_OEM_PERIOD]			=	KEY_OEM_PERIOD;
	data[0][VK_OEM_2]				=	KEY_OEM_2;
	data[0][VK_OEM_3]				=	KEY_OEM_3;
	data[0][VK_OEM_4]				=	KEY_OEM_4;
	data[0][VK_OEM_5]				=	KEY_OEM_5;
	data[0][VK_OEM_6]				=	KEY_OEM_6;
	data[0][VK_OEM_7]				=	KEY_OEM_7;
	data[0][VK_OEM_8]				=	KEY_OEM_8;
	data[0][VK_OEM_AX]				=	KEY_OEM_AX;
	data[0][VK_OEM_102]				=	KEY_OEM_102;
	data[0][VK_ICO_HELP]			=	KEY_ICO_HELP;
	data[0][VK_ICO_00]				=	KEY_ICO_00;
	data[0][VK_PROCESSKEY]			=	KEY_PROCESSKEY;
	data[0][VK_ICO_CLEAR]			=	KEY_ICO_CLEAR;
	data[0][VK_PACKET]				=	KEY_PACKET;
	data[0][VK_OEM_RESET]			=	KEY_OEM_RESET;
	data[0][VK_OEM_JUMP]			=	KEY_OEM_JUMP;
	data[0][VK_OEM_PA1]				=	KEY_OEM_PA1;
	data[0][VK_OEM_PA2]				=	KEY_OEM_PA2;
	data[0][VK_OEM_PA3]				=	KEY_OEM_PA3;
	data[0][VK_OEM_WSCTRL]			=	KEY_OEM_WSCTRL;
	data[0][VK_OEM_CUSEL]			=	KEY_OEM_CUSEL;
	data[0][VK_OEM_ATTN]			=	KEY_OEM_ATTN;
	data[0][VK_OEM_FINISH]			=	KEY_OEM_FINISH;
	data[0][VK_OEM_COPY]			=	KEY_OEM_COPY;
	data[0][VK_OEM_AUTO]			=	KEY_OEM_AUTO;
	data[0][VK_OEM_ENLW]			=	KEY_OEM_ENLW;
	data[0][VK_OEM_BACKTAB]			=	KEY_OEM_BACKTAB;
	data[0][VK_ATTN]				=	KEY_ATTN;
	data[0][VK_CRSEL]				=	KEY_CRSEL;
	data[0][VK_EXSEL]				=	KEY_EXSEL;
	data[0][VK_EREOF]				=	KEY_EREOF;
	data[0][VK_PLAY]				=	KEY_PLAY;
	data[0][VK_ZOOM]				=	KEY_ZOOM;
	data[0][VK_NONAME]				=	KEY_NONAME;
	data[0][VK_PA1]					=	KEY_PA1;
	data[0][VK_OEM_CLEAR]			=	KEY_OEM_CLEAR;
	data[0]['0']					=	KEY_0;
	data[0]['1']					=	KEY_1;
	data[0]['2']					=	KEY_2;
	data[0]['3']					=	KEY_3;
	data[0]['4']					=	KEY_4;
	data[0]['5']					=	KEY_5;
	data[0]['6']					=	KEY_6;
	data[0]['7']					=	KEY_7;
	data[0]['8']					=	KEY_8;
	data[0]['9']					=	KEY_9;
	data[0]['A']					=	KEY_A;
	data[0]['B']					=	KEY_B;
	data[0]['C']					=	KEY_C;
	data[0]['D']					=	KEY_D;
	data[0]['E']					=	KEY_E;
	data[0]['F']					=	KEY_F;
	data[0]['G']					=	KEY_G;
	data[0]['H']					=	KEY_H;
	data[0]['I']					=	KEY_I;
	data[0]['J']					=	KEY_J;
	data[0]['K']					=	KEY_K;
	data[0]['L']					=	KEY_L;
	data[0]['M']					=	KEY_M;
	data[0]['N']					=	KEY_N;
	data[0]['O']					=	KEY_O;
	data[0]['P']					=	KEY_P;
	data[0]['Q']					=	KEY_Q;
	data[0]['R']					=	KEY_R;
	data[0]['S']					=	KEY_S;
	data[0]['T']					=	KEY_T;
	data[0]['U']					=	KEY_U;
	data[0]['V']					=	KEY_V;
	data[0]['W']					=	KEY_W;
	data[0]['X']					=	KEY_X;
	data[0]['Y']					=	KEY_Y;
	data[0]['Z']					=	KEY_Z;			  
}			  
		
		



WinInput::WinInput()
{
	ENFORCE(!current);
	current = this;
}

void WinInput::initDirectInput()
{
	InitDirectInput();	
}

void WinInput::updateDirectInput()
{
	UpdateInputState(data.directInput2[0]);
}

void WinInput::updatePreviousState()
{
	data.mouse[1]			= data.mouse[0];
	data.keyboard[1]		= data.keyboard[0];
	data.xpad[1]			= data.xpad[0];
	data.directInput2[1]	= data.directInput2[0];
}

void WinInput::keyboardProc(int charCode, bool pressed)
{
	static KeyboardDict dict;

	data.keyboard[0](0, dict(0, charCode)) = pressed;
}
												 
void WinInput::mouseProc(bool leftButtonDown, bool rightButtonDown, bool middleButtonDown, bool sideButton1Down, bool sideButton2Down, float wheelDelta, float xPos, float yPos)
{
	static float wheelDeltaValue = 0.0f;
	wheelDeltaValue += wheelDelta; 

	data.mouse[0](0, MOUSE_BUTTON_LEFT)		= leftButtonDown	? 1.0f : 0.0f;
	data.mouse[0](0, MOUSE_BUTTON_RIGHT)	= rightButtonDown	? 1.0f : 0.0f;
	data.mouse[0](0, MOUSE_BUTTON_MIDDLE)	= middleButtonDown	? 1.0f : 0.0f;
	data.mouse[0](0, MOUSE_BUTTON_X1)		= sideButton1Down	? 1.0f : 0.0f;
	data.mouse[0](0, MOUSE_BUTTON_X2)		= sideButton2Down	? 1.0f : 0.0f;

	data.mouse[0](0, MOUSE_AXIS_X)			= xPos;
	data.mouse[0](0, MOUSE_AXIS_Y)			= yPos;
	data.mouse[0](0, MOUSE_AXIS_Z)			= wheelDeltaValue;
}

void WinInput::init()
{
	mem::memzero(xpadControllers);
	initDirectInput();
}

void WinInput::xpadProc()
{
	
}

void WinInput::update()
{
	updatePreviousState();
	xpadProc();
	updateDirectInput();
}

float WinInput::keyboard_impl(KeyCode code, float& prev)
{
	prev = data.keyboard[1](0, code);
 	return data.keyboard[0](0, code);
}

float WinInput::mouse_impl(MouseCode code, float& prev)
{
	prev = data.mouse[1](0, code);
	return data.mouse[0](0, code);
}

float WinInput::xpad_impl(unsigned int index, XPadCode code, float& prev)
{
	prev = data.xpad[1](index, code);
	return data.xpad[0](index, code);
}

float WinInput::directpad2_impl(DirectInputCode2 code, float& prev)
{
	prev = data.directInput2[1](0, code);
	return data.directInput2[0](0, code);
}

}

#endif




// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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