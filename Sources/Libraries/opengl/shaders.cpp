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

			// Info log length includes the null terminator, so 1 means that the info log is an empty  string.
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
		//create();

		// 	if (transformFeedbackVaryings.size() > 0)
		// 	{
		// 		std::vector<const char *> constCharTFVaryings;
		// 
		// 		for (const std::string &transformFeedbackVarying : transformFeedbackVaryings)
		// 		{
		// 			constCharTFVaryings.push_back(transformFeedbackVarying.c_str());
		// 		}
		// 
		// 		glTransformFeedbackVaryings(program, static_cast<GLsizei>(transformFeedbackVaryings.size()), &constCharTFVaryings[0], bufferMode);
		// 	}
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

			// Info log length includes the null terminator, so 1 means that the info log is an empty string.
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
		//LOG_EXPRESSION(program);
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

		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)
		const GLsizei bufSize = 64; // maximum name length
		GLchar uniname[bufSize]; // variable name in GLSL
		GLsizei length; // name length

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

	//
	//
	//
	std::unique_ptr<Uniform> constructUniform(GLenum type)
	{
		Uniform* result = nullptr;
		switch (type)
		{
		case GL_FLOAT:	result = new UniformT<GL_FLOAT>(); break;
		case GL_FLOAT_VEC2:	result = new UniformT<GL_FLOAT_VEC2>(); break;
		case GL_FLOAT_VEC3:	result = new UniformT<GL_FLOAT_VEC3>(); break;
		case GL_FLOAT_VEC4:	result = new UniformT<GL_FLOAT_VEC4>(); break;
// 		case GL_FLOAT_MAT2:	result = new UniformT<GL_FLOAT_MAT2>(); break;
// 		case GL_FLOAT_MAT3:	result = new UniformT<GL_FLOAT_MAT3>(); break;
// 		case GL_FLOAT_MAT4:	result = new UniformT<GL_FLOAT_MAT4>(); break;
// 		case GL_FLOAT_MAT2x3:	result = new UniformT<GL_FLOAT_MAT2x3>(); break;
// 		case GL_FLOAT_MAT2x4:	result = new UniformT<GL_FLOAT_MAT2x4>(); break;
// 		case GL_FLOAT_MAT3x2:	result = new UniformT<GL_FLOAT_MAT3x2>(); break;
// 		case GL_FLOAT_MAT3x4:	result = new UniformT<GL_FLOAT_MAT3x4>(); break;
// 		case GL_FLOAT_MAT4x2:	result = new UniformT<GL_FLOAT_MAT4x2>(); break;
// 		case GL_FLOAT_MAT4x3:	result = new UniformT<GL_FLOAT_MAT4x3>(); break;
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
// 		case GL_DOUBLE_MAT2:	result = new UniformT<GL_DOUBLE_MAT2>(); break;
// 		case GL_DOUBLE_MAT3:	result = new UniformT<GL_DOUBLE_MAT3>(); break;
// 		case GL_DOUBLE_MAT4:	result = new UniformT<GL_DOUBLE_MAT4>(); break;
// 		case GL_DOUBLE_MAT2x3:	result = new UniformT<GL_DOUBLE_MAT2x3>(); break;
// 		case GL_DOUBLE_MAT2x4:	result = new UniformT<GL_DOUBLE_MAT2x4>(); break;
// 		case GL_DOUBLE_MAT3x2:	result = new UniformT<GL_DOUBLE_MAT3x2>(); break;
// 		case GL_DOUBLE_MAT3x4:	result = new UniformT<GL_DOUBLE_MAT3x4>(); break;
// 		case GL_DOUBLE_MAT4x2:	result = new UniformT<GL_DOUBLE_MAT4x2>(); break;
// 		case GL_DOUBLE_MAT4x3:	result = new UniformT<GL_DOUBLE_MAT4x3>(); break;
		}

		return std::unique_ptr<Uniform>(result);
	}

	//
	//
	//
	void ShaderProgramCompiler::compile(ShaderProgram& program)
	{
		program.create();

		for (auto& shader : shaders)
		{
			program.attach(shader.first, std::get<0>(shader.second), std::get<1>(shader.second));
		}

		//program.transformFeedback(transformFeedbackVaryings, bufferMode);
		program.link();

		
		const GLsizei bufSize = 64; // maximum name length
		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)				
		GLchar name[bufSize]; // variable name in GLSL
		GLsizei length; // name length

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
		
		boost::replace_all(src, "#line", "//line");

		

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



