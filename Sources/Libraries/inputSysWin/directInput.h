#pragma once

#ifdef WIN32

#include "inputSysWin.h"

namespace Input
{
	HRESULT InitDirectInput();
	HRESULT UpdateInputState(WinInput::DirectInput2Buffer& buffer);
	void FreeDirectInput();
}

#endif