#include "InputSystem.h"
#include <sstream>

namespace Input
{

	/*
	Примеры консольных команд

	bind action key_a mod_lshift
	bind action mouse_x
	bind action gpad_axis_x3 mod_lshift
	*/

	// конвертация строка <-> InputSystem::enum`s, 0 - не найден идентификатор
	std::string InputModifierToStr(int modifiers)	
	{
		std::string res;

		if (modifiers & MOD_KEY_LCONTROL) res += CONVERT_ENUM_TO_STR(InputModifier, MOD_KEY_LCONTROL);
		if (modifiers & MOD_KEY_RCONTROL) res += CONVERT_ENUM_TO_STR(InputModifier, MOD_KEY_RCONTROL);
		if (modifiers & MOD_KEY_LALT) res += CONVERT_ENUM_TO_STR(InputModifier, MOD_KEY_LALT);
		if (modifiers & MOD_KEY_RALT) res += CONVERT_ENUM_TO_STR(InputModifier, MOD_KEY_RALT);
		if (modifiers & MOD_KEY_LWIN) res += CONVERT_ENUM_TO_STR(InputModifier, MOD_KEY_LWIN);
		if (modifiers & MOD_KEY_RWIN) res += CONVERT_ENUM_TO_STR(InputModifier, MOD_KEY_RWIN);
		if (modifiers & MOD_KEY_LSHIFT) res += CONVERT_ENUM_TO_STR(InputModifier, MOD_KEY_LSHIFT);
		if (modifiers & MOD_KEY_RSHIFT) res += CONVERT_ENUM_TO_STR(InputModifier, MOD_KEY_RSHIFT);
		if (modifiers & MOD_KEY_CAPS_LOCK) res += CONVERT_ENUM_TO_STR(InputModifier, MOD_KEY_CAPS_LOCK);
		if (modifiers & MOD_MOUSE_BUTTON_LEFT) res += CONVERT_ENUM_TO_STR(InputModifier, MOD_MOUSE_BUTTON_LEFT);
		if (modifiers & MOD_MOUSE_BUTTON_RIGHT) res += CONVERT_ENUM_TO_STR(InputModifier, MOD_MOUSE_BUTTON_RIGHT);
		if (modifiers & MOD_MOUSE_BUTTON_MIDDLE) res += CONVERT_ENUM_TO_STR(InputModifier, MOD_MOUSE_BUTTON_MIDDLE);
		if (modifiers & MOD_MOUSE_BUTTON_X1) res += CONVERT_ENUM_TO_STR(InputModifier, MOD_MOUSE_BUTTON_X1);
		if (modifiers & MOD_MOUSE_BUTTON_X2) res += CONVERT_ENUM_TO_STR(InputModifier, MOD_MOUSE_BUTTON_X2);
		
		return res; 
	}

	InputModifier StrToInputModifier(const char* str)	
	{ 
		return CONVERT_STR_TO_ENUM(InputModifier, str); 
	}

	std::string InputEventToStr(InputEvent ev)
	{
		int code = ev.code();
		InputEvent::DEVICE_TYPE_FLAGS device = ev.device();
		std::string s;
		switch(device)
		{
		case InputEvent::KEYBOARD:	s = CONVERT_ENUM_TO_STR(KeyCode		, (KeyCode		)code ); break;
		case InputEvent::MOUSE:		s = CONVERT_ENUM_TO_STR(MouseCode	, (MouseCode	)code ); break;
		case InputEvent::XPAD:		s = CONVERT_ENUM_TO_STR(XPadCode	, (XPadCode		)code ); break;
		};

		// если это не геймпад
		if(device != InputEvent::XPAD)
			return s;

		// добавить индекс для геймпада
		return str::stringize(s, ev.port());
	}

	InputEvent StrToInputEvent(const std::string& str)
	{
		if(str.empty())	{	return InputEvent();	}

		
		// если это геймпад
		if(str.find("XPAD_") != std::string::npos)
		{
			std::string s = str;

			// читаем цифру - порт в конце
			int port = str[str.size() - 1] - '0';

			s[str.size() - 1] = 0;

			return InputEvent(CONVERT_STR_TO_ENUM(XPadCode, s.c_str()), port);
		}

		// если это мышка
		if(str.find("MOUSE_") != std::string::npos)
		{
			return InputEvent(CONVERT_STR_TO_ENUM(MouseCode, str.c_str()));
		}

		// клавиша
		return InputEvent(CONVERT_STR_TO_ENUM(KeyCode, str.c_str()));
	}

	BEGIN_ENUM_STR_TABLE(InputModifier)
	{	MOD_NONE				,	""					},
	{	MOD_KEY_LCONTROL		,	"LCONTROL"			},
	{	MOD_KEY_RCONTROL		,	"RCONTROL"			},
	{	MOD_KEY_LALT			,	"LALT" 				},
	{	MOD_KEY_RALT			,	"RALT" 				},
	{	MOD_KEY_LWIN			,	"LWIN" 				},
	{	MOD_KEY_RWIN			,	"RWIN" 				},
	{	MOD_KEY_LSHIFT			,	"LSHIFT"			},
	{	MOD_KEY_RSHIFT			,	"RSHIFT"			},
	{	MOD_KEY_CAPS_LOCK		,	"CAPSLOCK" 			},
	{	MOD_MOUSE_BUTTON_LEFT	,	"MOUSE_LB" 			},
	{	MOD_MOUSE_BUTTON_RIGHT	,	"MOUSE_RB" 			},
	{	MOD_MOUSE_BUTTON_MIDDLE	,	"MOUSE_MB" 			},
	{	MOD_MOUSE_BUTTON_X1		,	"MOUSE_X1" 			},
	{	MOD_MOUSE_BUTTON_X2		,	"MOUSE_X2" 			},
	END_ENUM_STR_TABLE(InputModifier);


	BEGIN_ENUM_STR_TABLE(KeyCode)
	{	ERROR_KEYBOARD_CODE		, "ERROR_KEYBOARD_CODE"	},
	{	KEY_LBUTTON        		, "LBUTTON"				},
	{	KEY_RBUTTON        		, "RBUTTON"				},
	{	KEY_CANCEL         		, "CANCEL"				},
	{	KEY_MBUTTON        		, "MBUTTON"				},
	{	KEY_XBUTTON1       		, "XBUTTON1"			},
	{	KEY_XBUTTON2       		, "XBUTTON2"			},
	{	KEY_BACK           		, "BACK"				},
	{	KEY_TAB            		, "TAB"					},
	{	KEY_CLEAR          		, "CLEAR"				},
	{	KEY_RETURN         		, "RETURN"				},
	{	KEY_SHIFT          		, "SHIFT"				},
	{	KEY_CONTROL        		, "CONTROL"				},
	{	KEY_MENU           		, "MENU"				},
	{	KEY_PAUSE          		, "PAUSE"				},
	{	KEY_CAPITAL        		, "CAPITAL"				},
	{	KEY_KANA           		, "KANA"				},
	{	KEY_HANGEUL        		, "HANGEUL"				},
	{	KEY_HANGUL         		, "HANGUL"				},
	{	KEY_JUNJA          		, "JUNJA"				},
	{	KEY_FINAL          		, "FINAL"				},
	{	KEY_HANJA          		, "HANJA"				},
	{	KEY_KANJI          		, "KANJI"				},
	{	KEY_ESCAPE         		, "ESCAPE"				},
	{	KEY_CONVERT        		, "CONVERT"				},
	{	KEY_NONCONVERT     		, "NONCONVERT"			},
	{	KEY_ACCEPT         		, "ACCEPT"				},
	{	KEY_MODECHANGE     		, "MODECHANGE"			},
	{	KEY_SPACE          		, "SPACE"				},
	{	KEY_PRIOR          		, "PRIOR"				},
	{	KEY_NEXT           		, "NEXT"				},
	{	KEY_END            		, "END"					},
	{	KEY_HOME           		, "HOME"				},
	{	KEY_LEFT           		, "LEFT"				},
	{	KEY_UP             		, "UP"					},
	{	KEY_RIGHT          		, "RIGHT"				},
	{	KEY_DOWN           		, "DOWN"				},
	{	KEY_SELECT         		, "SELECT"				},
	{	KEY_PRINT          		, "PRINT"				},
	{	KEY_EXECUTE        		, "EXECUTE"				},
	{	KEY_SNAPSHOT       		, "SNAPSHOT"			},
	{	KEY_INSERT         		, "INSERT"				},
	{	KEY_DELETE         		, "DELETE"				},
	{	KEY_HELP           		, "HELP"				},
	{	KEY_LWIN           		, "LWIN"				},
	{	KEY_RWIN           		, "RWIN"				},
	{	KEY_APPS           		, "APPS"				},
	{	KEY_SLEEP          		, "SLEEP"				},
	{	KEY_NUMPAD0        		, "NUMPAD0"				},
	{	KEY_NUMPAD1        		, "NUMPAD1"				},
	{	KEY_NUMPAD2        		, "NUMPAD2"				},
	{	KEY_NUMPAD3        		, "NUMPAD3"				},
	{	KEY_NUMPAD4        		, "NUMPAD4"				},
	{	KEY_NUMPAD5        		, "NUMPAD5"				},
	{	KEY_NUMPAD6        		, "NUMPAD6"				},
	{	KEY_NUMPAD7        		, "NUMPAD7"				},
	{	KEY_NUMPAD8        		, "NUMPAD8"				},
	{	KEY_NUMPAD9        		, "NUMPAD9"				},
	{	KEY_MULTIPLY       		, "MULTIPLY"			},
	{	KEY_ADD            		, "ADD"					},
	{	KEY_SEPARATOR      		, "SEPARATOR"			},
	{	KEY_SUBTRACT       		, "SUBTRACT"			},
	{	KEY_DECIMAL        		, "DECIMAL"				},
	{	KEY_DIVIDE         		, "DIVIDE"				},
	{	KEY_F1             		, "F1"					},
	{	KEY_F2             		, "F2"					},
	{	KEY_F3             		, "F3"					},
	{	KEY_F4             		, "F4"					},
	{	KEY_F5             		, "F5"					},
	{	KEY_F6             		, "F6"					},
	{	KEY_F7             		, "F7"					},
	{	KEY_F8             		, "F8"					},
	{	KEY_F9             		, "F9"					},
	{	KEY_F10            		, "F10"					},
	{	KEY_F11            		, "F11"					},
	{	KEY_F12            		, "F12"					},
	{	KEY_F13            		, "F13"					},
	{	KEY_F14            		, "F14"					},
	{	KEY_F15            		, "F15"					},
	{	KEY_F16            		, "F16"					},
	{	KEY_F17            		, "F17"					},
	{	KEY_F18            		, "F18"					},
	{	KEY_F19            		, "F19"					},
	{	KEY_F20            		, "F20"					},
	{	KEY_F21            		, "F21"					},
	{	KEY_F22            		, "F22"					},
	{	KEY_F23            		, "F23"					},
	{	KEY_F24            		, "F24"					},
	{	KEY_NUMLOCK        		, "NUMLOCK"				},
	{	KEY_SCROLL         		, "SCROLL"				},
	{	KEY_OEM_NEC_EQUAL  		, "OEM_NEC_EQUAL"		},
	{	KEY_OEM_FJ_JISHO   		, "OEM_FJ_JISHO"		},
	{	KEY_OEM_FJ_MASSHOU 		, "OEM_FJ_MASSHOU"		},
	{	KEY_OEM_FJ_TOUROKU 		, "OEM_FJ_TOUROKU"		},
	{	KEY_OEM_FJ_LOYA    		, "OEM_FJ_LOYA"			},
	{	KEY_OEM_FJ_ROYA    		, "OEM_FJ_ROYA"			},
	{	KEY_LSHIFT         		, "LSHIFT"				},
	{	KEY_RSHIFT         		, "RSHIFT"				},
	{	KEY_LCONTROL       		, "LCONTROL"			},
	{	KEY_RCONTROL       		, "RCONTROL"			},
	{	KEY_LMENU          		, "LMENU"				},
	{	KEY_RMENU          		, "RMENU"				},
	{ 	KEY_BROWSER_BACK        , "BROWSER_BACK"		},
	{ 	KEY_BROWSER_FORWARD     , "BROWSER_FORWARD"		},
	{ 	KEY_BROWSER_REFRESH     , "BROWSER_REFRESH"		},
	{ 	KEY_BROWSER_STOP        , "BROWSER_STOP"		},
	{ 	KEY_BROWSER_SEARCH      , "BROWSER_SEARCH"		},
	{ 	KEY_BROWSER_FAVORITES   , "BROWSER_FAVORITES"	},
	{ 	KEY_BROWSER_HOME        , "BROWSER_HOME"		},
	{ 	KEY_VOLUME_MUTE         , "VOLUME_MUTE"			},
	{ 	KEY_VOLUME_DOWN         , "VOLUME_DOWN"			},
	{ 	KEY_VOLUME_UP           , "VOLUME_UP"			},
	{ 	KEY_MEDIA_NEXT_TRACK    , "MEDIA_NEXT_TRACK"	},
	{ 	KEY_MEDIA_PREV_TRACK    , "MEDIA_PREV_TRACK"	},
	{ 	KEY_MEDIA_STOP          , "MEDIA_STOP"			},
	{ 	KEY_MEDIA_PLAY_PAUSE    , "MEDIA_PLAY_PAUSE"	},
	{ 	KEY_LAUNCH_MAIL         , "LAUNCH_MAIL"			},
	{ 	KEY_LAUNCH_MEDIA_SELECT , "LAUNCH_MEDIA_SELECT"	},
	{ 	KEY_LAUNCH_APP1         , "LAUNCH_APP1"			},
	{ 	KEY_LAUNCH_APP2         , "LAUNCH_APP2"			},
	{	KEY_OEM_1          		, "OEM_1"				},
	{	KEY_OEM_PLUS       		, "OEM_PLUS"			},
	{	KEY_OEM_COMMA      		, "OEM_COMMA"			},
	{	KEY_OEM_MINUS      		, "OEM_MINUS"			},
	{	KEY_OEM_PERIOD     		, "OEM_PERIOD"			},
	{	KEY_OEM_2          		, "OEM_2"				},
	{	KEY_OEM_3          		, "OEM_3"				},
	{	KEY_OEM_4          		, "OEM_4"				},
	{	KEY_OEM_5          		, "OEM_5"				},
	{	KEY_OEM_6          		, "OEM_6"				},
	{	KEY_OEM_7          		, "OEM_7"				},
	{	KEY_OEM_8          		, "OEM_8"				},
	{	KEY_OEM_AX         		, "OEM_AX"				},
	{	KEY_OEM_102        		, "OEM_102"				},
	{	KEY_ICO_HELP       		, "ICO_HELP"			},
	{	KEY_ICO_00         		, "ICO_00"				},
	{	KEY_PROCESSKEY     		, "PROCESSKEY"			},
	{	KEY_ICO_CLEAR      		, "ICO_CLEAR"			},
	{	KEY_PACKET         		, "PACKET"				},
	{	KEY_OEM_RESET      		, "OEM_RESET"			},
	{	KEY_OEM_JUMP       		, "OEM_JUMP"			},
	{	KEY_OEM_PA1        		, "OEM_PA1"				},
	{	KEY_OEM_PA2        		, "OEM_PA2"				},
	{	KEY_OEM_PA3        		, "OEM_PA3"				},
	{	KEY_OEM_WSCTRL     		, "OEM_WSCTRL"			},
	{	KEY_OEM_CUSEL      		, "OEM_CUSEL"			},
	{	KEY_OEM_ATTN       		, "OEM_ATTN"			},
	{	KEY_OEM_FINISH     		, "OEM_FINISH"			},
	{	KEY_OEM_COPY       		, "OEM_COPY"			},
	{	KEY_OEM_AUTO       		, "OEM_AUTO"			},
	{	KEY_OEM_ENLW       		, "OEM_ENLW"			},
	{	KEY_OEM_BACKTAB    		, "OEM_BACKTAB"			},
	{	KEY_ATTN           		, "ATTN"				},
	{	KEY_CRSEL          		, "CRSEL"				},
	{	KEY_EXSEL          		, "EXSEL"				},
	{	KEY_EREOF          		, "EREOF"				},
	{	KEY_PLAY           		, "PLAY"				},
	{	KEY_ZOOM           		, "ZOOM"				},
	{	KEY_NONAME         		, "NONAME"				},
	{	KEY_PA1            		, "PA1"					},
	{	KEY_OEM_CLEAR      		, "OEM_CLEAR"			},
	{ 	KEY_0					, "0"					},
	{ 	KEY_1					, "1"					},
	{ 	KEY_2					, "2"					},
	{ 	KEY_3					, "3"					},
	{ 	KEY_4					, "4"					},
	{ 	KEY_5					, "5"					},
	{ 	KEY_6					, "6"					},
	{ 	KEY_7					, "7"					},
	{ 	KEY_8					, "8"					},
	{ 	KEY_9					, "9"					},
	{ 	KEY_A					, "A"					},
	{ 	KEY_B					, "B"					},
	{ 	KEY_C					, "C"					},
	{	KEY_D 					, "D"					},
	{	KEY_E 					, "E"					},
	{	KEY_F 					, "F"					},
	{	KEY_G 					, "G"					},
	{	KEY_H 					, "H"					},
	{	KEY_I 					, "I"					},
	{	KEY_J 					, "J"					},
	{	KEY_K 					, "K"					},
	{	KEY_L 					, "L"					},
	{	KEY_M 					, "M"					},
	{	KEY_N 					, "N"					},
	{	KEY_O 					, "O"					},
	{	KEY_P 					, "P"					},
	{	KEY_Q 					, "Q"					},
	{	KEY_R 					, "R"					},
	{	KEY_S 					, "S"					},
	{	KEY_T 					, "T"					},
	{	KEY_U 					, "U"					},
	{	KEY_V 					, "V"					},
	{	KEY_W 					, "W"					},
	{	KEY_X 					, "X"					},
	{	KEY_Y 					, "Y"					},
	{	KEY_Z 					, "Z"					},
	END_ENUM_STR_TABLE(KeyCode);


	BEGIN_ENUM_STR_TABLE(MouseCode)
	{ ERROR_MOUSE_CODE			,	"ERROR_MOUSE_CODE"	},
	{ MOUSE_BUTTON_LEFT			,	"MOUSE_LB"			},
	{ MOUSE_BUTTON_RIGHT		,	"MOUSE_RB" 			},
	{ MOUSE_BUTTON_MIDDLE		,	"MOUSE_MB" 			},
	{ MOUSE_BUTTON_X1			,	"MOUSE_X1" 			},
	{ MOUSE_BUTTON_X2			,	"MOUSE_X2" 			},
	{ MOUSE_BUTTON_X3			,	"MOUSE_X3" 			},
	{ MOUSE_BUTTON_X4			,	"MOUSE_X4" 			},
	{ MOUSE_BUTTON_X5			,	"MOUSE_X5" 			},
	{ MOUSE_AXIS_X				,	"MOUSE_AXIS_X" 		},
	{ MOUSE_AXIS_Y				,	"MOUSE_AXIS_Y" 		},
	{ MOUSE_AXIS_Z				,	"MOUSE_AXIS_Z" 		},
	END_ENUM_STR_TABLE(MouseCode);

	// коды геймпада
	BEGIN_ENUM_STR_TABLE(XPadCode)
		{ XPAD_DPAD_UP,					"XPAD_DPAD_UP"},
		{ XPAD_DPAD_DOWN,				"XPAD_DPAD_DOWN"},
		{ XPAD_DPAD_LEFT,				"XPAD_DPAD_LEFT"},
		{ XPAD_DPAD_RIGHT,				"XPAD_DPAD_RIGHT"},

		{ XPAD_BUTTON_A,				"XPAD_BUTTON_A"},
		{ XPAD_BUTTON_B,				"XPAD_BUTTON_B"},
		{ XPAD_BUTTON_X,				"XPAD_BUTTON_X"},
		{ XPAD_BUTTON_Y,				"XPAD_BUTTON_Y"},

		{ XPAD_BUTTON_LEFT_THUMB,		"XPAD_BUTTON_LEFT_THUMB"},
		{ XPAD_BUTTON_RIGHT_THUMB,		"XPAD_BUTTON_RIGHT_THUMB"},

		{ XPAD_BUTTON_LEFT_SHOULDER,	"XPAD_BUTTON_LEFT_SHOULDER"},
		{ XPAD_BUTTON_RIGHT_SHOULDER,	"XPAD_BUTTON_RIGHT_SHOULDER"},

		{ XPAD_BUTTON_START,			"XPAD_BUTTON_START"},
		{ XPAD_BUTTON_BACK,				"XPAD_BUTTON_BACK"},

		{ XPAD_AXIS_LEFT_STICK_X,		"XPAD_AXIS_LEFT_STICK_X"},
		{ XPAD_AXIS_LEFT_STICK_Y,		"XPAD_AXIS_LEFT_STICK_Y"},
		{ XPAD_AXIS_RIGHT_STICK_X,		"XPAD_AXIS_RIGHT_STICK_X"},
		{ XPAD_AXIS_RIGHT_STICK_Y,		"XPAD_AXIS_RIGHT_STICK_Y"},

		{ XPAD_AXIS_LEFT_TRIGGER,		"XPAD_AXIS_LEFT_TRIGGER"},
		{ XPAD_AXIS_RIGHT_TRIGGER,		"XPAD_AXIS_RIGHT_TRIGGER"},

	END_ENUM_STR_TABLE(XPadCode);



// 	BEGIN_ENUM_STR_TABLE(GamePadCode)
// 	{ ERROR_GAMEPAD_CODE		,	"ERROR_GAMEPAD_CODE"},
// 	{ GAMEPAD_BUTTON_LUP		,	"GPAD_LUP"			},
// 	{ GAMEPAD_BUTTON_LDOWN		,	"GPAD_LDOWN" 		},
// 	{ GAMEPAD_BUTTON_LLEFT		,	"GPAD_LLEFT" 		},
// 	{ GAMEPAD_BUTTON_LRIGHT		,	"GPAD_LRIGHT"		},
// 	
// 	{ GAMEPAD_BUTTON_L1			,	"GPAD_L1" 			},
// 	{ GAMEPAD_BUTTON_L2			,	"GPAD_L2" 			},
// 	{ GAMEPAD_BUTTON_R1			,	"GPAD_R1" 			},
// 	{ GAMEPAD_BUTTON_R2			,	"GPAD_R2" 			},
// 
// 	{ GAMEPAD_BUTTON_RUP		,	"GPAD_RUP"			},
// 	{ GAMEPAD_BUTTON_RDOWN		,	"GPAD_RDOWN"		},
// 	{ GAMEPAD_BUTTON_RLEFT		,	"GPAD_RLEFT"		},
// 	{ GAMEPAD_BUTTON_RRIGHT		,	"GPAD_RRIGHT"		},
// 
// 	{ GAMEPAD_STICK_BUTTON_A	,	"GPAD_A" 			},
// 	{ GAMEPAD_STICK_BUTTON_B	,	"GPAD_B" 			},
// 	
// 	{ GAMEPAD_BUTTON_START		,	"GPAD_START"		},
// 	{ GAMEPAD_BUTTON_SELECT		,	"GPAD_SELECT"		},
// 
// 	{ GAMEPAD_STICK_AXIS_LEFT_X	,	"GPAD_AXISLX" 		},
// 	{ GAMEPAD_STICK_AXIS_LEFT_Y	,	"GPAD_AXISLY" 		},
// 	{ GAMEPAD_STICK_AXIS_RIGHT_X,	"GPAD_RIGHTX" 		},
// 	{ GAMEPAD_STICK_AXIS_RIGHT_Y,	"GPAD_RIGHTY" 		},
// 	END_ENUM_STR_TABLE(GamePadCode);

}