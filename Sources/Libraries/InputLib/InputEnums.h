#pragma once

#include "stuff/EnumStrTable.h"

#ifdef KEY_EXECUTE
#undef KEY_EXECUTE
#endif
// enum коснтанты для Input системы
namespace Input
{
	enum KeyCode
	{
		ERROR_KEYBOARD_CODE,
		KEY_LBUTTON        			,
		KEY_RBUTTON        			,
		KEY_CANCEL         			,
		KEY_MBUTTON        			,
		KEY_XBUTTON1       			,
		KEY_XBUTTON2       			,
		KEY_BACK           			,
		KEY_TAB            			,
		KEY_CLEAR          			,
		KEY_RETURN         			,
		KEY_SHIFT          			,
		KEY_CONTROL        			,
		KEY_MENU           			,
		KEY_PAUSE          			,
		KEY_CAPITAL        			,
		KEY_KANA           			,
		KEY_HANGEUL        			,
		KEY_HANGUL         			,
		KEY_JUNJA          			,
		KEY_FINAL          			,
		KEY_HANJA          			,
		KEY_KANJI          			,
		KEY_ESCAPE         			,
		KEY_CONVERT        			,
		KEY_NONCONVERT     			,
		KEY_ACCEPT         			,
		KEY_MODECHANGE     			,
		KEY_SPACE          			,
		KEY_PRIOR          			,
		KEY_NEXT           			,
		KEY_END            			,
		KEY_HOME           			,
		KEY_LEFT           			,
		KEY_UP             			,
		KEY_RIGHT          			,
		KEY_DOWN           			,
		KEY_SELECT         			,
		KEY_PRINT          			,
		KEY_EXECUTE        			,
		KEY_SNAPSHOT       			,
		KEY_INSERT         			,
		KEY_DELETE         			,
		KEY_HELP           			,
		KEY_LWIN           			,
		KEY_RWIN           			,
		KEY_APPS           			,
		KEY_SLEEP          			,
		KEY_NUMPAD0        			,
		KEY_NUMPAD1        			,
		KEY_NUMPAD2        			,
		KEY_NUMPAD3        			,
		KEY_NUMPAD4        			,
		KEY_NUMPAD5        			,
		KEY_NUMPAD6        			,
		KEY_NUMPAD7        			,
		KEY_NUMPAD8        			,
		KEY_NUMPAD9        			,
		KEY_MULTIPLY       			,
		KEY_ADD            			,
		KEY_SEPARATOR      			,
		KEY_SUBTRACT       			,
		KEY_DECIMAL        			,
		KEY_DIVIDE         			,
		KEY_F1             			,
		KEY_F2             			,
		KEY_F3             			,
		KEY_F4             			,
		KEY_F5             			,
		KEY_F6             			,
		KEY_F7             			,
		KEY_F8             			,
		KEY_F9             			,
		KEY_F10           			,
		KEY_F11            			,
		KEY_F12            			,
		KEY_F13            			,
		KEY_F14            			,
		KEY_F15            			,
		KEY_F16            			,
		KEY_F17            			,
		KEY_F18            			,
		KEY_F19            			,
		KEY_F20            			,
		KEY_F21            			,
		KEY_F22            			,
		KEY_F23            			,
		KEY_F24            			,
		KEY_NUMLOCK        			,
		KEY_SCROLL         			,
		KEY_OEM_NEC_EQUAL  			,
		KEY_OEM_FJ_JISHO   			,
		KEY_OEM_FJ_MASSHOU 			,
		KEY_OEM_FJ_TOUROKU 			,
		KEY_OEM_FJ_LOYA    			,
		KEY_OEM_FJ_ROYA    			,
		KEY_LSHIFT         			,
		KEY_RSHIFT         			,
		KEY_LCONTROL       			,
		KEY_RCONTROL       			,
		KEY_LMENU          			,
		KEY_RMENU          			,
		KEY_BROWSER_BACK        	,
		KEY_BROWSER_FORWARD     	,
		KEY_BROWSER_REFRESH     	,
		KEY_BROWSER_STOP        	,
		KEY_BROWSER_SEARCH      	,
		KEY_BROWSER_FAVORITES   	,
		KEY_BROWSER_HOME        	,
		KEY_VOLUME_MUTE         	,
		KEY_VOLUME_DOWN         	,
		KEY_VOLUME_UP           	,
		KEY_MEDIA_NEXT_TRACK    	,
		KEY_MEDIA_PREV_TRACK    	,
		KEY_MEDIA_STOP          	,
		KEY_MEDIA_PLAY_PAUSE    	,
		KEY_LAUNCH_MAIL         	,
		KEY_LAUNCH_MEDIA_SELECT 	,
		KEY_LAUNCH_APP1         	,
		KEY_LAUNCH_APP2         	,
		KEY_OEM_1          			,
		KEY_OEM_PLUS       			,
		KEY_OEM_COMMA      			,
		KEY_OEM_MINUS      			,
		KEY_OEM_PERIOD     			,
		KEY_OEM_2          			,
		KEY_OEM_3          			,
		KEY_OEM_4          			,
		KEY_OEM_5          			,
		KEY_OEM_6          			,
		KEY_OEM_7          			,
		KEY_OEM_8          			,
		KEY_OEM_AX         			,
		KEY_OEM_102        			,
		KEY_ICO_HELP       			,
		KEY_ICO_00         			,
		KEY_PROCESSKEY     			,
		KEY_ICO_CLEAR      			,
		KEY_PACKET         			,
		KEY_OEM_RESET      			,
		KEY_OEM_JUMP       			,
		KEY_OEM_PA1        			,
		KEY_OEM_PA2        			,
		KEY_OEM_PA3        			,
		KEY_OEM_WSCTRL     			,
		KEY_OEM_CUSEL      			,
		KEY_OEM_ATTN       			,
		KEY_OEM_FINISH     			,
		KEY_OEM_COPY       			,
		KEY_OEM_AUTO       			,
		KEY_OEM_ENLW       			,
		KEY_OEM_BACKTAB    			,
		KEY_ATTN           			,
		KEY_CRSEL          			,
		KEY_EXSEL          			,
		KEY_EREOF          			,
		KEY_PLAY           			,
		KEY_ZOOM           			,
		KEY_NONAME         			,
		KEY_PA1            			,
		KEY_OEM_CLEAR      			,
		KEY_0           			,
		KEY_1             			,
		KEY_2             			,
		KEY_3             			,
		KEY_4             			,
		KEY_5             			,
		KEY_6             			,
		KEY_7             			,
		KEY_8             			,
		KEY_9             			,
		KEY_A           			,
		KEY_B             			,
		KEY_C             			,
		KEY_D             			,
		KEY_E             			,
		KEY_F             			,
		KEY_G             			,
		KEY_H             			,
		KEY_I             			,
		KEY_J             			,
		KEY_K           			,
		KEY_L             			,
		KEY_M             			,
		KEY_N             			,
		KEY_O             			,
		KEY_P             			,
		KEY_Q             			,
		KEY_R             			,
		KEY_S             			,
		KEY_T             			,
		KEY_U           			,
		KEY_V             			,
		KEY_W             			,
		KEY_X             			,
		KEY_Y             			,
		KEY_Z             			,
		KEYBOARD_TOTAL_COUNT
	};						

	// коды мышки			
	enum MouseCode			
	{					
		ERROR_MOUSE_CODE,
		MOUSE_BUTTON_LEFT,
		MOUSE_BUTTON_RIGHT,
		MOUSE_BUTTON_MIDDLE,
		MOUSE_BUTTON_X1,		// до 5ти кнопок
		MOUSE_BUTTON_X2,	
		MOUSE_BUTTON_X3,	
		MOUSE_BUTTON_X4,	
		MOUSE_BUTTON_X5,	

		MOUSE_AXIS_X,			// оси
		MOUSE_AXIS_Y,		
		MOUSE_AXIS_Z,			// колёсико

		MOUSE_TOTAL_COUNT,

		MOUSE_AXIS_COUNT = 3,
		MOUSE_BUTTON_COUNT = 8,
	};			

#define XINPUT_GAMEPAD_DPAD_UP          0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN        0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT        0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
#define XINPUT_GAMEPAD_START            0x0010
#define XINPUT_GAMEPAD_BACK             0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB       0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
#define XINPUT_GAMEPAD_A                0x1000
#define XINPUT_GAMEPAD_B                0x2000
#define XINPUT_GAMEPAD_X                0x4000
#define XINPUT_GAMEPAD_Y                0x8000

	// коды геймпада		
	enum XPadCode		
	{					
		XPAD_DPAD_UP, 
		XPAD_DPAD_DOWN, 
		XPAD_DPAD_LEFT, 
		XPAD_DPAD_RIGHT, 

		XPAD_BUTTON_A,
		XPAD_BUTTON_B,
		XPAD_BUTTON_X,
		XPAD_BUTTON_Y,

		XPAD_BUTTON_LEFT_THUMB, 
		XPAD_BUTTON_RIGHT_THUMB, 

		XPAD_BUTTON_LEFT_SHOULDER, 		
		XPAD_BUTTON_RIGHT_SHOULDER, 
								  
		XPAD_BUTTON_START, 
		XPAD_BUTTON_BACK, 

		XPAD_AXIS_LEFT_STICK_X,
		XPAD_AXIS_LEFT_STICK_Y,
		XPAD_AXIS_RIGHT_STICK_X,
		XPAD_AXIS_RIGHT_STICK_Y,

		XPAD_AXIS_LEFT_TRIGGER, 
		XPAD_AXIS_RIGHT_TRIGGER,

		XPAD_TOTAL_COUNT,

		XPAD_BUTTON_COUNT = 14,
		XPAD_AXIS_COUNT = 6,
	};						

	// коды геймпада		
	enum DirectInputCode2		
	{		
		DIRECT_PAD2_X_POSITION,                     /* x-axis position              */
		DIRECT_PAD2_Y_POSITION,                     /* y-axis position              */
		DIRECT_PAD2_Z_POSITION,                     /* z-axis position              */
		DIRECT_PAD2_X_ROTATION,                    /* x-axis rotation              */
		DIRECT_PAD2_Y_ROTATION,                    /* y-axis rotation              */
		DIRECT_PAD2_Z_ROTATION,                    /* z-axis rotation              */
		DIRECT_PAD2_SLIDER0_POSITION,
		DIRECT_PAD2_SLIDER1_POSITION,	/* extra axes positions         */
		DIRECT_PAD2_POINT_OF_VIEW0,		/* POV directions               */
		DIRECT_PAD2_POINT_OF_VIEW1,
		DIRECT_PAD2_POINT_OF_VIEW2,
		DIRECT_PAD2_POINT_OF_VIEW3,
		DIRECT_PAD2_BUTTON12,
		DIRECT_PAD2_BUTTON13,
		
// 			BYTE    rgbButtons[128];        /* 128 buttons                  */
// 			LONG    lVX;                    /* x-axis velocity              */
// 			LONG    lVY;                    /* y-axis velocity              */
// 			LONG    lVZ;                    /* z-axis velocity              */
// 			LONG    lVRx;                   /* x-axis angular velocity      */
// 			LONG    lVRy;                   /* y-axis angular velocity      */
// 			LONG    lVRz;                   /* z-axis angular velocity      */
// 			LONG    rglVSlider[2];          /* extra axes velocities        */
// 			LONG    lAX;                    /* x-axis acceleration          */
// 			LONG    lAY;                    /* y-axis acceleration          */
// 			LONG    lAZ;                    /* z-axis acceleration          */
// 			LONG    lARx;                   /* x-axis angular acceleration  */
// 			LONG    lARy;                   /* y-axis angular acceleration  */
// 			LONG    lARz;                   /* z-axis angular acceleration  */
// 			LONG    rglASlider[2];          /* extra axes accelerations     */
// 			LONG    lFX;                    /* x-axis force                 */
// 			LONG    lFY;                    /* y-axis force                 */
// 			LONG    lFZ;                    /* z-axis force                 */
// 			LONG    lFRx;                   /* x-axis torque                */
// 			LONG    lFRy;                   /* y-axis torque                */
// 			LONG    lFRz;                   /* z-axis torque                */
// 			LONG    rglFSlider[2];          /* extra axes forces            */

		DIRECT_PAD2_TOTAL_COUNT,

		//DIRECT_PAD2_BUTTON_COUNT = 14,
		//DIRECT_PAD2_AXIS_COUNT = 6,
	};

	// модификаторы			
	enum InputModifier		
	{						
		MOD_NONE				= 0,
		MOD_KEY_LCONTROL	 	= 1 << 0,
		MOD_KEY_RCONTROL	 	= 1 << 1,
		MOD_KEY_LALT    	 	= 1 << 2,
		MOD_KEY_RALT    	 	= 1 << 3,
		MOD_KEY_LWIN    	 	= 1 << 4,
		MOD_KEY_RWIN    	 	= 1 << 5,
		MOD_KEY_LSHIFT  	 	= 1 << 6,
		MOD_KEY_RSHIFT  	 	= 1 << 7,
		MOD_KEY_CAPS_LOCK		= 1 << 8,
		MOD_MOUSE_BUTTON_LEFT	= 1 << 9,
		MOD_MOUSE_BUTTON_RIGHT	= 1 << 10,
		MOD_MOUSE_BUTTON_MIDDLE = 1 << 11,
		MOD_MOUSE_BUTTON_X1		= 1 << 12,
		MOD_MOUSE_BUTTON_X2		= 1 << 13,

		MOD_KEY_COUNT = 9,
	};		

	// таблицы конвертации
	DECLARE_ENUM_STR_TABLE(InputModifier);
	DECLARE_ENUM_STR_TABLE(KeyCode);
	DECLARE_ENUM_STR_TABLE(MouseCode);
	DECLARE_ENUM_STR_TABLE(XPadCode);
}	