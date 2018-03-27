#pragma once

namespace opengl
{
	void check(const char* location);
}

#define GL_CALL(FUNC) do{ FUNC; opengl::check(SOURCE_LOCATION); } while(0);




