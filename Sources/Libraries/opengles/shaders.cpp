#include "shaders.h"
#include "defaultLogs/library.include.h"

#include <vector>

ShaderProgram::~ShaderProgram()
{
	destroy();
}

void ShaderProgram::attach(GLenum type, const std::string& source)
{
	create();

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
			LOG_ERROR("shader compilation failed: " << info);
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
	create();

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

	return program;
}

void ShaderProgram::create()
{
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

GLint ShaderProgram::uniform(GLchar* name) const
{
	GLint res = glGetUniformLocation(program, name);
	opengl::check(SOURCE_LOCATION);
	return res;
}

GLint ShaderProgram::attrib(GLchar* name) const
{
	return glGetAttribLocation(program, name);
}

//
//
//
void ShaderProgramCompiler::compile(ShaderProgram& program)
{
	program.destroy();

	for (auto& shader : shaders)
	{
		program.attach(shader.first, shader.second);
	}

	program.transformFeedback(transformFeedbackVaryings, bufferMode);
	program.link();
}

void ShaderProgramCompiler::attach(GLenum type, const std::string& source)
{
	shaders[type] = source;
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