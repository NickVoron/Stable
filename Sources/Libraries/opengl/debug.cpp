#include "debug.h"

namespace opengl
{
	const char* errorType(GLenum code)
	{
		switch (code)
		{
		case GL_INVALID_ENUM: 					return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE:					return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION:				return "GL_INVALID_OPERATION";
		case GL_OUT_OF_MEMORY:					return "GL_OUT_OF_MEMORY";
		case GL_INVALID_FRAMEBUFFER_OPERATION:	return "GL_INVALID_FRAMEBUFFER_OPERATION";
#if ENABLE_OPENGL
		case GL_TABLE_TOO_LARGE: 				return "GL_TABLE_TOO_LARGE";
		case GL_STACK_OVERFLOW:					return "GL_STACK_OVERFLOW";
		case GL_STACK_UNDERFLOW:				return "GL_STACK_UNDERFLOW";
#endif	
		default:								return "unknown";
		}

		return "unknown";
	}

	void check(const char* location)
	{
		static bool prev = false;
		auto code = glGetError();
 		if(code != GL_NO_ERROR)
 		{
			//if (!prev)
			{
				//LOG_ERROR_UNDECORATED(location << " OpenGL error " << errorType(code));
				prev = true;
			} 			
 		}
	}
}