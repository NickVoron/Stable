#include "commands.h"
#include "InputSystem.h"

namespace Input
{
	/*
	void CommandBindInputAction::execute()
	{
		const std::string& actionName	= getArgVal<std::string>(0);
		const std::string& keyName		= getArgVal<std::string>(1);

		InputEvent event = InputSystem::StrToInputEvent(keyName);

		bool succeded = true;
		int count = arguments.size();
		count -= 2;
		int modifiers = 0;
		for (int i = 0; i < count; ++i)
		{
			const std::string& modCode = getArgVal<std::string>(2 + i);
			InputModifier modifier = InputSystem::StrToInputModifier( modCode.c_str() );
			if(!modCode.empty() && modifier == MOD_NONE)
			{
				succeded = false;
				LOG_INFO("modifier code: " << modCode << " is ivalid, use command \"show_input_modifiers\" for help ");
			}

			modifiers |= modifier;
		}

		if(succeded)
		{
			action(actionName.c_str(), event, (InputModifier)modifiers);
		}		
	}

	void CommandBindInputAction::fillArgs()
	{
		arguments.push_back( new CmdProc::CommandArg<std::string>("actionName", "action name used in code") );
		arguments.push_back( new CmdProc::CommandArg<std::string>("keyName", "keyboard, mouse or gamepad key") );

		arguments.push_back( new CmdProc::CommandArg<std::string>("modifier1" , "modifier1", ""  ) );
		arguments.push_back( new CmdProc::CommandArg<std::string>("modifier2" , "modifier2", ""  ) );
		arguments.push_back( new CmdProc::CommandArg<std::string>("modifier3" , "modifier3", ""  ) );
		arguments.push_back( new CmdProc::CommandArg<std::string>("modifier4" , "modifier4", ""  ) );
		arguments.push_back( new CmdProc::CommandArg<std::string>("modifier5" , "modifier5", ""  ) );
		arguments.push_back( new CmdProc::CommandArg<std::string>("modifier6" , "modifier6", ""  ) );
		arguments.push_back( new CmdProc::CommandArg<std::string>("modifier7" , "modifier7", ""  ) );
		arguments.push_back( new CmdProc::CommandArg<std::string>("modifier8" , "modifier8", ""  ) );
		arguments.push_back( new CmdProc::CommandArg<std::string>("modifier9" , "modifier9", ""  ) );
		arguments.push_back( new CmdProc::CommandArg<std::string>("modifier10", "modifier10", "" ) );
		arguments.push_back( new CmdProc::CommandArg<std::string>("modifier11", "modifier11", "" ) );
		arguments.push_back( new CmdProc::CommandArg<std::string>("modifier12", "modifier12", "" ) );
		arguments.push_back( new CmdProc::CommandArg<std::string>("modifier13", "modifier13", "" ) );
		arguments.push_back( new CmdProc::CommandArg<std::string>("modifier14", "modifier14", "" ) );
		arguments.push_back( new CmdProc::CommandArg<std::string>("modifier15", "modifier15", "" ) );
	}


	//
	void CommandShowInputModifiers::execute()
	{
		LOG_INFO(	InputSystem::InputModifierToStr(	MOD_KEY_LCONTROL	 	)	);
		LOG_INFO(	InputSystem::InputModifierToStr(	MOD_KEY_RCONTROL	 	)	);
		LOG_INFO(	InputSystem::InputModifierToStr(	MOD_KEY_LALT    	 	)	);
		LOG_INFO(	InputSystem::InputModifierToStr(	MOD_KEY_RALT    	 	)	);
		LOG_INFO(	InputSystem::InputModifierToStr(	MOD_KEY_LWIN    	 	)	);
		LOG_INFO(	InputSystem::InputModifierToStr(	MOD_KEY_RWIN    	 	)	);
		LOG_INFO(	InputSystem::InputModifierToStr(	MOD_KEY_LSHIFT  	 	)	);
		LOG_INFO(	InputSystem::InputModifierToStr(	MOD_KEY_RSHIFT  	 	)	);
		LOG_INFO(	InputSystem::InputModifierToStr(	MOD_KEY_CAPS_LOCK		)	);
		LOG_INFO(	InputSystem::InputModifierToStr(	MOD_MOUSE_BUTTON_LEFT	)	);
		LOG_INFO(	InputSystem::InputModifierToStr(	MOD_MOUSE_BUTTON_RIGHT	)	);
		LOG_INFO(	InputSystem::InputModifierToStr(	MOD_MOUSE_BUTTON_MIDDLE	)	);
		LOG_INFO(	InputSystem::InputModifierToStr(	MOD_MOUSE_BUTTON_X1		)	);
		LOG_INFO(	InputSystem::InputModifierToStr(	MOD_MOUSE_BUTTON_X2		)	);

		//const std::string& fileName = getArgVal<std::string>(0);
	}

	void CommandShowInputModifiers::fillArgs()
	{
		//arguments.push_back( new CmdProc::CommandArg<std::string>("fn", "file name") );
	}

	//
	//
	//
	void CommandShowKeyboardCodes::execute()
	{
		for (int i = 1; i < KEYBOARD_TOTAL_COUNT; ++i)
		{
			LOG_INFO(	CONVERT_ENUM_TO_STR(KeyCode, (KeyCode)i)	);
		}
	}

	void CommandShowKeyboardCodes::fillArgs()
	{
	}

	//
	//
	//
	void CommandShowMouseCodes::execute()
	{
		for (int i = 1; i < MOUSE_TOTAL_COUNT; ++i)
		{
			LOG_INFO(	CONVERT_ENUM_TO_STR(MouseCode, (MouseCode)i)	);
		}
	}

	void CommandShowMouseCodes::fillArgs()
	{
	}

	//
	//
	//
	void CommandShowGamepadCodes::execute()
	{
		for (int i = 1; i < GAMEPAD_TOTAL_COUNT; ++i)
		{
			LOG_INFO(	CONVERT_ENUM_TO_STR(GamePadCode, (GamePadCode)i)	);
		}
	}

	void CommandShowGamepadCodes::fillArgs()
	{
	}


	void registerInputCommands()
	{
		CmdProc::registerCommand<CommandBindInputAction>();

		CmdProc::registerCommand<CommandShowInputModifiers>();
		CmdProc::registerCommand<CommandShowKeyboardCodes>();
		CmdProc::registerCommand<CommandShowMouseCodes>();
		CmdProc::registerCommand<CommandShowGamepadCodes>();
	}

	*/
}