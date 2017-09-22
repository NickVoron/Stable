// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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