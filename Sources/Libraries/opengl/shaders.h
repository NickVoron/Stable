// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "memory/library.include.h"
#include "textFile/library.include.h"
#include "gapi/library.include.h"

#include "debug.h"

#if ENABLE_OPENGL

namespace opengl
{
	

	struct ShaderBase 
	{
		~ShaderBase() {	clear(); }

		void clear()
		{
			glDeleteShader(shader);
			check();
		}

		GLuint shader;
	};

	template<GLenum shaderType> 
	struct ShaderDef : public ShaderBase
	{
		void load(const char* fileName)
		{
			TextFile::TextFromFile text(fileName);
			const char* shaderSource;
			int shaderLen;
			text.text(&shaderSource, &shaderLen);

 			shader = glCreateShader(shaderType);
 			glShaderSource(shader, 1, (const GLchar**)&shaderSource, (const GLint*)&shaderLen);
			glCompileShader(shader);
			check();
		}
	};

 	template<GLenum shaderType> 
 	struct Shader : public ShaderDef< gapi::ShaderType<shaderType>::type >
 	{
 
 	};

	struct ShaderProgram
	{
		void create();
		void destroy();
		void attach(const ShaderBase& shader);
		void link();
		void set();

		GLuint program;
	};

}

#endif



// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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