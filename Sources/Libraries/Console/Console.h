#pragma once

#include <string>

namespace Base{

struct Console
{
	enum Colors
	{
		COLOR_EMPTY		= 0x0000,
		COLOR_RED		= 0x0001,
		COLOR_GREEN		= 0x0002,
		COLOR_BLUE		= 0x0004,
		COLOR_INTENSITY	= 0x0008,
		COLOR_WHITE		= COLOR_RED | COLOR_GREEN | COLOR_BLUE
	};

	static void create();
	static void destroy();

	static void color(bool background, int c0 = COLOR_WHITE);
	static void colorReset();
	
	static std::string command();
};

}
