#pragma once

#include <vector>
#include <string>

#include "InputEnums.h"

#include "stuff/library.include.h"

namespace Input
{
	// тип для преобразования клавиш осей разных типов в одно пространство
	class InputEvent
	{
	public:
		// флаги типа устройства для Action
		enum DEVICE_TYPE_FLAGS
		{
			KEYBOARD	= 0x00100000,	// клавиш
			MOUSE		= 0x00200000,	// мышка
			XPAD		= 0x00400000,	// геймпад XBox360
			DIPAD		= 0x00800000,	// геймпад DirectInput

			TYPE_MASK	= 0x00F00000,	// маска типа
			CODE_MASK	= 0x0000FFFF,	// маска кода
			PORT_MASK	= 0xFF000000,	// маска номера порта

			FORCE_DWORD	= 0x7FFFFFFF,
		};

		InputEvent();
		InputEvent(KeyCode c);
		InputEvent(MouseCode c);
		InputEvent(DirectInputCode2 c);
		InputEvent(XPadCode c, int index);
		

		DEVICE_TYPE_FLAGS device() const;
		int	code() const;
		int port() const;

		float value(float& prev) const;
		float value() const;

	private:
		int data;
	};

	//
	//
	//
	class Action
	{
	public:
		InputEvent event;
		int modifier = MOD_NONE;

		Action(){}
		Action(const InputEvent& event, int modifier = MOD_NONE){ init(event, modifier); }
		void init(const InputEvent& event, int modifier = MOD_NONE);
		bool pressed(bool now = false) const;
		bool released(bool now = false) const;

		float value() const;
		float delta() const;
		void report() const;

	private:
		bool state(bool pressed, bool now) const;
	};
	
	// система ввода
	class InputSystem
	{
	public:
		static InputSystem* get();

		InputSystem();
		bool enabled() const;
		void enabled(bool e);

		float keyboard(KeyCode code);
		float mouse(MouseCode code);
		float directpad2(DirectInputCode2 code);
		float xpad(unsigned int index, XPadCode code);		
		
		float keyboard(KeyCode code, float& prev);
		float mouse(MouseCode code, float& prev);
		float directpad2(DirectInputCode2 code, float& prev);
		float xpad(unsigned int index, XPadCode code, float& prev);

	protected:
		virtual float keyboard_impl(KeyCode code, float& prev)					= 0;
		virtual float mouse_impl(MouseCode code, float& prev)					= 0;
		virtual float directpad2_impl(DirectInputCode2 code, float& prev)		= 0;
		virtual float xpad_impl(unsigned int index, XPadCode code, float& prev) = 0;

		static InputSystem* current;

		bool enabledValue;
	};

	float mouseX();
	float mouseY();
	float mouseZ();

	bool mouseL(bool now = false);
	bool mouseM(bool now = false);
	bool mouseR(bool now = false);	
	bool mouseX1(bool now = false);
	bool mouseX2(bool now = false);
	bool mouseX3(bool now = false);
	bool mouseX4(bool now = false);
	bool mouseX5(bool now = false);

	bool keyboardCTRL(bool now = false);
	bool keyboardSHIFT(bool now = false);

	bool xpadA(int index, bool now = false);
	bool xpadB(int index, bool now = false);
	bool xpadX(int index, bool now = false);
	bool xpadY(int index, bool now = false);

	bool checkModifier(int mod);

	template<class CodeType> bool universal(int deviceIndex, CodeType code, float& prev, float& current);

	template<class CodeType>
	bool universal(int deviceIndex, CodeType code, bool now)
	{
		float prev = 0.0f, value = 0.0f;
		bool res = universal(deviceIndex, code, prev, value);
		return res && (((value != 0.0f) && !now) || (((value != 0.0f) && now && (value != prev))));
	}
	
	bool mouse(MouseCode code, bool now = false);
	bool keyboard(KeyCode code, bool now = false);
	bool xpad(int index, XPadCode code, bool now = false);
	bool directpad2(DirectInputCode2 code, bool now = false);

	template<class CodeType> bool switcher(CodeType code, bool& value)				{ if (universal(0, code, true)) value = !value; 	return value; }
	template<class CodeType> bool switcher(int index, CodeType code, bool& value)	{ if (universal(index, code, true)) value = !value; return value; }

	template<class CodeType, class ValueType> 
	ValueType& switcher(CodeType code, ValueType& value, const ValueType& option0, const ValueType& option1)
	{ 
		if (!(value == option0) && !(value == option1)) value = option0;
		
		if (universal(0, code, true))
		{
			if (value == option0) { value = option1; return value; }
			if (value == option1) { value = option0; return value; }
		}		

		return value;
	}

	template<class CodeType, class ValueType>
	ValueType& switcher(CodeType code, ValueType& value, const ValueType& option0, const ValueType& option1, const ValueType& option2)
	{
		if (!(value == option0) && !(value == option1) && !(value == option2)) value = option0;

		if (universal(0, code, true))
		{
			if (value == option0) { value = option1; return value; }
			if (value == option1) { value = option2; return value; }
			if (value == option2) { value = option0; return value; }
		}

		return value;
	}

	template<class CodeType, class ValueType>
	ValueType& switcher(CodeType code, ValueType& value, const ValueType& option0, const ValueType& option1, const ValueType& option2, const ValueType& option3)
	{
		if (!(value == option0) && !(value == option1) && !(value == option2) && !(value == option3)) value = option0;

		if (universal(0, code, true))
		{
			if (value == option0) { value = option1; return value; }
			if (value == option1) { value = option2; return value; }
			if (value == option2) { value = option3; return value; }
			if (value == option3) { value = option0; return value; }
		}

		return value;
	}

	template<class CodeType, class Function> void activator(CodeType code, const Function& function) { if (universal(0, code, false)) function(); }
	
	void inputProcessed(bool processed);

	template<class CodeType> Action action(CodeType code, int modifiers = MOD_NONE) { return Action(code, modifiers); }

	Action action(int index, XPadCode code, int modifiers = MOD_NONE);

	template<class CodeType> inline float value(CodeType code, int modifiers = MOD_NONE) { return action(code, modifiers).value(); }
	template<class CodeType> inline float delta(CodeType code, int modifiers = MOD_NONE) { return action(code, modifiers).delta(); }

	inline float value(int index, XPadCode code, int modifiers = MOD_NONE) { return action(index, code, modifiers).value(); }
	inline float delta(int index, XPadCode code, int modifiers = MOD_NONE) { return action(index, code, modifiers).delta(); }
}