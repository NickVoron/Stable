#pragma once

#include "InputLib/library.include.h"

#ifdef WIN32

#include <XInput.h> 
#include <dinput.h>

namespace Input
{

class WinInput : public InputSystem
{
public: 
	WinInput();

	void keyboardProc(int charCode, bool pressed);
	void mouseProc(bool leftButtonDown, bool rightButtonDown, bool middleButtonDown, bool sideButton1Down, bool sideButton2Down, float wheelDelta, float xPos, float yPos);
	void xpadProc();
	void init();
	
	virtual void update();

	virtual float keyboard_impl(KeyCode code, float& prev);
	virtual float mouse_impl(MouseCode code, float& prev);
	virtual float xpad_impl(unsigned int index, XPadCode code, float& prev);
	virtual float directpad2_impl(DirectInputCode2 code, float& prev);

	template<class Type, int deviceCount, int count>
	struct InputBuffer
	{
		static const int DEVICE_COUNT = deviceCount;
		static const int ELEMENTS_COUNT = count;

		InputBuffer(){ mem::memzero(data); }
		inline Type&	operator()(unsigned int index, unsigned int i)			{ ENFORCE((i < ELEMENTS_COUNT) && (index < DEVICE_COUNT)); return data[index][i]; }
		inline Type		operator()(unsigned int index, unsigned int i) const	{ ENFORCE((i < ELEMENTS_COUNT) && (index < DEVICE_COUNT)); return data[index][i]; }

		Type data[DEVICE_COUNT][ELEMENTS_COUNT];
	};

	struct XPadBuffer			: public InputBuffer<float, 4, XPAD_TOTAL_COUNT>		{};
	struct MouseBuffer			: public InputBuffer<float, 1, MOUSE_TOTAL_COUNT> 		{};
	struct DirectInput2Buffer	: public InputBuffer<float, 1, DIRECT_PAD2_TOTAL_COUNT> {};
	struct KeyboardBuffer		: public InputBuffer<bool, 1, 256> 						{};

private:
	void updatePreviousState();
	void initDirectInput();
	void updateDirectInput();



	struct KeyboardDict		: public InputBuffer<KeyCode, 1, 256> 		
	{
		KeyboardDict();
	};

	struct Data 
	{
		MouseBuffer mouse[2];
		KeyboardBuffer keyboard[2];
		XPadBuffer xpad[2];
		DirectInput2Buffer directInput2[2];
	};

	Data data;	

	struct CONTROLER_STATE
	{
		XINPUT_STATE state;
		bool connected;
	};

	CONTROLER_STATE xpadControllers[XPadBuffer::DEVICE_COUNT];
};

}

#endif
