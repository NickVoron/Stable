#pragma once

#include "stream/library.include.h"
#include "opengl/library.include.h"
#include "glew/library.include.h"

#include <string>
#include <vector>

template<class T, int count = sizeof(T) / sizeof(GLfloat)> struct Uniformf;
template<class T, int count = sizeof(T) / sizeof(GLint)> struct Uniformi;

template<class T> struct Uniformf<T, 1> { static void call(GLuint location, const T& value) { GL_CALL(glUniform1f(location, value)); } };
template<class T> struct Uniformf<T, 2> { static void call(GLuint location, const T& value) { GL_CALL(glUniform2fv(location, 1, (GLfloat*) &value)); } };
template<class T> struct Uniformf<T, 3> { static void call(GLuint location, const T& value) { GL_CALL(glUniform3fv(location, 1, (GLfloat*) &value)); } };
template<class T> struct Uniformf<T, 4> { static void call(GLuint location, const T& value) { GL_CALL(glUniform4fv(location, 1, (GLfloat*) &value)); } };

template<class T> struct Uniformi<T, 1> { static void call(GLuint location, const T& value) { GL_CALL(glUniform1i(location, value)); } };
template<class T> struct Uniformi<T, 2> { static void call(GLuint location, const T& value) { GL_CALL(glUniform2iv(location, 1, (GLint*) &value)); } };
template<class T> struct Uniformi<T, 3> { static void call(GLuint location, const T& value) { GL_CALL(glUniform3iv(location, 1, (GLint*) &value)); } };
template<class T> struct Uniformi<T, 4> { static void call(GLuint location, const T& value) { GL_CALL(glUniform4iv(location, 1, (GLint*) &value)); } };


template<class T> void uniformf(GLuint location, const T& value) { Uniformf<T>::call(location, value); }
template<class T> void uniformi(GLuint location, const T& value) { Uniformi<T>::call(location, value); }

struct ShaderProgram
{
	~ShaderProgram();

	void attach(GLenum type, const std::string& source);
	void transformFeedback(const std::vector<std::string> &transformFeedbackVaryings, GLenum bufferMode);
	GLuint link();
	void create();
	void destroy();
	void set() const;

	GLint uniform(GLchar* name) const;
	GLint attrib(GLchar* name) const;

	GLuint program = 0;
};

class ShaderProgramCompiler 
{
public:
	void compile(ShaderProgram& program);
	void attach(GLenum type, const std::string& source);
	void transformFeedback(const std::vector<std::string> &transformFeedbackVaryings, GLenum bufferMode);
	void serialize(serializer& os);

private:
	std::map<GLenum, std::string> shaders;
	std::vector<std::string> transformFeedbackVaryings;
	GLenum bufferMode;
};