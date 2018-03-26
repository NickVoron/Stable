// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "shaders.h"
#include "debug.h"

#include <boost/algorithm/string/replace.hpp>

#if ENABLE_OPENGL

namespace opengl
{
	ShaderProgram::~ShaderProgram()
	{
		destroy();
	}

	void ShaderProgram::attach(GLenum type, const std::string& source, const std::string& fileName)
	{
		GLuint shader = glCreateShader(type);

		const char *sourceArray[1] = { source.c_str() };
		glShaderSource(shader, 1, sourceArray, NULL);
		glCompileShader(shader);

		GLint compileResult;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

		if (compileResult == 0)
		{
			GLint infoLogLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

			
			if (infoLogLength > 1)
			{
				std::vector<GLchar> infoLog(infoLogLength);
				glGetShaderInfoLog(shader, static_cast<GLsizei>(infoLog.size()), NULL, &infoLog[0]);
				std::string info = &infoLog[0];
				boost::replace_all(info, "\r\n", "\n");
				boost::replace_all(info, "\n", str::stringize("\n", fileName, "(0): ").str());
				LOG_ERROR(info);
			}
			else
			{
				LOG_ERROR("shader compilation failed. <Empty log message>");
			}

			glDeleteShader(shader);
			shader = 0;
		}

		if (shader)
		{
			glAttachShader(program, shader);
		}

		glDeleteShader(shader);
	}

	void ShaderProgram::transformFeedback(const std::vector<std::string> &transformFeedbackVaryings, GLenum bufferMode)
	{
		

		
		
		
		
		
		
		
		
		
		
		
	}

	GLuint ShaderProgram::link()
	{
		glLinkProgram(program);

		GLint linkStatus;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

		if (linkStatus == 0)
		{
			GLint infoLogLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

			
			if (infoLogLength > 1)
			{
				std::vector<GLchar> infoLog(infoLogLength);
				glGetProgramInfoLog(program, static_cast<GLsizei>(infoLog.size()), nullptr, &infoLog[0]);

				LOG_ERROR("program link failed: " << &infoLog[0]);
			}
			else
			{
				LOG_ERROR("program link failed. <Empty log message>");
			}

			glDeleteProgram(program);
			return 0;
		}

		LOG_MSG("link " << program);
		return program;
	}

	void ShaderProgram::create()
	{
		destroy();
		if (!program)
		{
			program = glCreateProgram();
		}
	}

	void ShaderProgram::destroy()
	{
		if (program)
		{
			glDeleteProgram(program);
			program = 0;
		}
	}

	void ShaderProgram::set() const
	{
		
		glUseProgram(program);
	}

	GLint ShaderProgram::uniformLocation(const GLchar* name) const
	{
		GLint res = glGetUniformLocation(program, name);
		opengl::check(SOURCE_LOCATION);
		return res;
	}

	GLint ShaderProgram::attrib(const GLchar* name) const
	{
		return glGetAttribLocation(program, name);
	}

	ConstantInfo ShaderProgram::uniformInfo(const GLchar* name) const
	{
		GLint count;
		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);

		GLint size; 
		GLenum type; 
		const GLsizei bufSize = 64; 
		GLchar uniname[bufSize]; 
		GLsizei length; 

		for (GLint i = 0; i < count; i++)
		{
			glGetActiveUniform(program, (GLuint) i, bufSize, &length, &size, &type, uniname);

			if (strcmp(name, uniname) == 0)
			{
				return {uniformLocation(name), type};
			}
		}

		return { -1, 0 };
	}

	
	
	
	std::unique_ptr<Uniform> constructUniform(GLenum type)
	{
		Uniform* result = nullptr;
		switch (type)
		{
		case GL_FLOAT:	result = new UniformT<GL_FLOAT>(); break;
		case GL_FLOAT_VEC2:	result = new UniformT<GL_FLOAT_VEC2>(); break;
		case GL_FLOAT_VEC3:	result = new UniformT<GL_FLOAT_VEC3>(); break;
		case GL_FLOAT_VEC4:	result = new UniformT<GL_FLOAT_VEC4>(); break;









		case GL_INT:	result = new UniformT<GL_INT>(); break;
		case GL_INT_VEC2:	result = new UniformT<GL_INT_VEC2>(); break;
		case GL_INT_VEC3:	result = new UniformT<GL_INT_VEC3>(); break;
		case GL_INT_VEC4:	result = new UniformT<GL_INT_VEC4>(); break;
		case GL_UNSIGNED_INT:	result = new UniformT<GL_UNSIGNED_INT>(); break;
		case GL_UNSIGNED_INT_VEC2:	result = new UniformT<GL_UNSIGNED_INT_VEC2>(); break;
		case GL_UNSIGNED_INT_VEC3:	result = new UniformT<GL_UNSIGNED_INT_VEC3>(); break;
		case GL_UNSIGNED_INT_VEC4:	result = new UniformT<GL_UNSIGNED_INT_VEC4>(); break;
		case GL_DOUBLE:	result = new UniformT<GL_DOUBLE>(); break;
		case GL_DOUBLE_VEC2:	result = new UniformT<GL_DOUBLE_VEC2>(); break;
		case GL_DOUBLE_VEC3:	result = new UniformT<GL_DOUBLE_VEC3>(); break;
		case GL_DOUBLE_VEC4:	result = new UniformT<GL_DOUBLE_VEC4>(); break;









		}

		return std::unique_ptr<Uniform>(result);
	}

	
	
	
	void ShaderProgramCompiler::compile(ShaderProgram& program)
	{
		program.create();

		for (auto& shader : shaders)
		{
			program.attach(shader.first, std::get<0>(shader.second), std::get<1>(shader.second));
		}

		
		program.link();

		
		const GLsizei bufSize = 64; 
		GLint size; 
		GLenum type; 
		GLchar name[bufSize]; 
		GLsizei length; 

		{
			GLint count;
			glGetProgramiv(program.program, GL_ACTIVE_ATTRIBUTES, &count);
			for (GLint i = 0; i < count; i++)
			{
				glGetActiveAttrib(program.program, (GLuint) i, bufSize, &length, &size, &type, name);
				auto location = glGetAttribLocation(program.program, name);
				attributes.push_back({ location, type });
			}
		}		

		{
			GLint count;
			glGetProgramiv(program.program, GL_ACTIVE_UNIFORMS, &count);
			for (GLint i = 0; i < count; i++)
			{
				glGetActiveUniform(program.program, (GLuint) i, bufSize, &length, &size, &type, name);
				auto location = glGetUniformLocation(program.program, name);
				uniforms.push_back({ location, type });
			}
		}
	}

	void ShaderProgramCompiler::attach(GLenum type, const std::string& source, const std::string& file, int versionTag)
	{
		std::string src;
		if (versionTag)
		{
			src += str::stringize("#version ", versionTag, "\n").str();
		}

		src += source;
		
		boost::replace_all(src, "#line", "

		

		shaders[type] = std::make_tuple(src, file);
	}

	void ShaderProgramCompiler::transformFeedback(const std::vector<std::string>& varyings, GLenum mode)
	{
		transformFeedbackVaryings = varyings;
		bufferMode = mode;
	}

	void ShaderProgramCompiler::serialize(serializer& os)
	{
		os << shaders << transformFeedbackVaryings << bufferMode;
	}
}

#endif







// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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