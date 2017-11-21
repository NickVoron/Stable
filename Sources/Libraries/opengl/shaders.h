#pragma once

#include "memory/library.include.h"
#include "textFile/library.include.h"
#include "gapi/library.include.h"

#include "debug.h"

#if ENABLE_OPENGL

namespace opengl
{
	//

	struct ShaderBase 
	{
		~ShaderBase() {	clear(); }

		void clear()
		{
			GL_CALL(glDeleteShader(shader));
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

	template<class T, int count = sizeof(T) / sizeof(GLfloat)> struct Uniformf;
	template<class T, int count = sizeof(T) / sizeof(GLint)> struct Uniformi;
	template<class T, int count = sizeof(T) / sizeof(GLint)> struct Uniformui;
	template<class T, int count = sizeof(T) / sizeof(GLdouble)> struct Uniformd;

	template<class T> struct Uniformd<T, 1> { static void call(GLuint location, const T& value) { GL_CALL(glUniform1d(location, value)); } };
	template<class T> struct Uniformd<T, 2> { static void call(GLuint location, const T& value) { GL_CALL(glUniform2dv(location, 1, (GLdouble*) &value)); } };
	template<class T> struct Uniformd<T, 3> { static void call(GLuint location, const T& value) { GL_CALL(glUniform3dv(location, 1, (GLdouble*) &value)); } };
	template<class T> struct Uniformd<T, 4> { static void call(GLuint location, const T& value) { GL_CALL(glUniform4dv(location, 1, (GLdouble*) &value)); } };

	template<class T> struct Uniformf<T, 1> { static void call(GLuint location, const T& value) { GL_CALL(glUniform1f(location, value)); } };
	template<class T> struct Uniformf<T, 2> { static void call(GLuint location, const T& value) { GL_CALL(glUniform2fv(location, 1, (GLfloat*) &value)); } };
	template<class T> struct Uniformf<T, 3> { static void call(GLuint location, const T& value) { GL_CALL(glUniform3fv(location, 1, (GLfloat*) &value)); } };
	template<class T> struct Uniformf<T, 4> { static void call(GLuint location, const T& value) 
	{ 
		GL_CALL(glUniform4fv(location, 1, (GLfloat*) &value[0])); 
	} 
	};

	template<class T> struct Uniformi<T, 1> { static void call(GLuint location, const T& value) { GL_CALL(glUniform1i(location, value)); } };
	template<class T> struct Uniformi<T, 2> { static void call(GLuint location, const T& value) { GL_CALL(glUniform2iv(location, 1, (GLint*) &value)); } };
	template<class T> struct Uniformi<T, 3> { static void call(GLuint location, const T& value) { GL_CALL(glUniform3iv(location, 1, (GLint*) &value)); } };
	template<class T> struct Uniformi<T, 4> { static void call(GLuint location, const T& value) { GL_CALL(glUniform4iv(location, 1, (GLint*) &value)); } };

	template<class T> struct Uniformui<T, 1> { static void call(GLuint location, const T& value) { GL_CALL(glUniform1ui(location, value)); } };
	template<class T> struct Uniformui<T, 2> { static void call(GLuint location, const T& value) { GL_CALL(glUniform2uiv(location, 1, (GLuint*) &value)); } };
	template<class T> struct Uniformui<T, 3> { static void call(GLuint location, const T& value) { GL_CALL(glUniform3uiv(location, 1, (GLuint*) &value)); } };
	template<class T> struct Uniformui<T, 4> { static void call(GLuint location, const T& value) { GL_CALL(glUniform4uiv(location, 1, (GLuint*) &value)); } };


	template<class T> void uniformd(GLuint location, const T& value) { Uniformd<T>::call(location, value); }
	template<class T> void uniformf(GLuint location, const T& value) { Uniformf<T>::call(location, value); }
	template<class T> void uniformi(GLuint location, const T& value) { Uniformi<T>::call(location, value); }
	template<class T> void uniformui(GLuint location, const T& value) { Uniformui<T>::call(location, value); }

	struct Uniform
	{
		virtual void upload(GLuint location) = 0;
		virtual void load(void* pdata, std::size_t size) = 0;
	};

	template<GLenum type> struct UniformHolder;

	template<> struct UniformHolder<GL_FLOAT>				{ void upload(GLuint location) { uniformf(location, data[0]); } float data[1]; };
	template<> struct UniformHolder<GL_FLOAT_VEC2>			{ void upload(GLuint location) { uniformf(location, data); } float data[2]; };
	template<> struct UniformHolder<GL_FLOAT_VEC3>			{ void upload(GLuint location) { uniformf(location, data); } float data[3]; };
	template<> struct UniformHolder<GL_FLOAT_VEC4>			{ void upload(GLuint location) { uniformf(location, data); } float data[4]; };
// 	template<> struct UniformHolder<GL_FLOAT_MAT2> { void upload(GLuint location) { uniformf(location, data); } float data[4]; };
// 	template<> struct UniformHolder<GL_FLOAT_MAT3> { void upload(GLuint location) { uniformf(location, data); } float data[9]; };
// 	template<> struct UniformHolder<GL_FLOAT_MAT4> { void upload(GLuint location) { uniformf(location, data); } float data[16]; };
// 	template<> struct UniformHolder<GL_FLOAT_MAT2x3> { void upload(GLuint location) { uniformf(location, data); } float data[6]; };
// 	template<> struct UniformHolder<GL_FLOAT_MAT2x4> { void upload(GLuint location) { uniformf(location, data); } float data[8]; };
// 	template<> struct UniformHolder<GL_FLOAT_MAT3x2> { void upload(GLuint location) { uniformf(location, data); } float data[6]; };
// 	template<> struct UniformHolder<GL_FLOAT_MAT3x4> { void upload(GLuint location) { uniformf(location, data); } float data[12]; };
// 	template<> struct UniformHolder<GL_FLOAT_MAT4x2> { void upload(GLuint location) { uniformf(location, data); } float data[8]; };
// 	template<> struct UniformHolder<GL_FLOAT_MAT4x3> { void upload(GLuint location) { uniformf(location, data); } float data[12]; };
	template<> struct UniformHolder<GL_INT>					{ void upload(GLuint location) { uniformi(location, data[0]); } int data[1]; };
	template<> struct UniformHolder<GL_INT_VEC2>			{ void upload(GLuint location) { uniformi(location, data); } int data[2]; };
	template<> struct UniformHolder<GL_INT_VEC3>			{ void upload(GLuint location) { uniformi(location, data); } int data[3]; };
	template<> struct UniformHolder<GL_INT_VEC4>			{ void upload(GLuint location) { uniformi(location, data); } int data[4]; };
	template<> struct UniformHolder<GL_UNSIGNED_INT>		{ void upload(GLuint location) { uniformui(location, data[0]); } unsigned int data[1]; };
	template<> struct UniformHolder<GL_UNSIGNED_INT_VEC2>	{ void upload(GLuint location) { uniformui(location, data); } unsigned int data[2]; };
	template<> struct UniformHolder<GL_UNSIGNED_INT_VEC3>	{ void upload(GLuint location) { uniformui(location, data); } unsigned int data[3]; };
	template<> struct UniformHolder<GL_UNSIGNED_INT_VEC4>	{ void upload(GLuint location) { uniformui(location, data); } unsigned int data[4]; };
	template<> struct UniformHolder<GL_DOUBLE>				{ void upload(GLuint location) { uniformd(location, data[0]); } double data[1]; };
	template<> struct UniformHolder<GL_DOUBLE_VEC2>			{ void upload(GLuint location) { uniformd(location, data); } double data[2]; };
	template<> struct UniformHolder<GL_DOUBLE_VEC3>			{ void upload(GLuint location) { uniformd(location, data); } double data[3]; };
 	template<> struct UniformHolder<GL_DOUBLE_VEC4>			{ void upload(GLuint location) { uniformd(location, data); } double data[4]; };
// 	template<> struct UniformHolder<GL_DOUBLE_MAT2> { void upload(GLuint location) { uniformd(location, data); } double data[4]; };
// 	template<> struct UniformHolder<GL_DOUBLE_MAT3> { void upload(GLuint location) { uniformd(location, data); } double data[9]; };
// 	template<> struct UniformHolder<GL_DOUBLE_MAT4> { void upload(GLuint location) { uniformd(location, data); } double data[16]; };
// 	template<> struct UniformHolder<GL_DOUBLE_MAT2x3> { void upload(GLuint location) { uniformd(location, data); } double data[6]; };
// 	template<> struct UniformHolder<GL_DOUBLE_MAT2x4> { void upload(GLuint location) { uniformd(location, data); } double data[8]; };
// 	template<> struct UniformHolder<GL_DOUBLE_MAT3x2> { void upload(GLuint location) { uniformd(location, data); } double data[6]; };
// 	template<> struct UniformHolder<GL_DOUBLE_MAT3x4> { void upload(GLuint location) { uniformd(location, data); } double data[12]; };
// 	template<> struct UniformHolder<GL_DOUBLE_MAT4x2> { void upload(GLuint location) { uniformd(location, data); } double data[8]; };
// 	template<> struct UniformHolder<GL_DOUBLE_MAT4x3> { void upload(GLuint location) { uniformd(location, data); } double data[12]; };



	template<GLenum type>
	struct UniformT : public Uniform, public UniformHolder<type>
	{
		virtual void upload(GLuint location) override
		{
			UniformHolder<type>::upload(location);
		}

		virtual void load(void* pdata, std::size_t size) override
		{
			auto sz = sizeof(data);
			ENFORCE_EQUAL(sz, size);
			memcpy(&data[0], pdata, sizeof(data));
		}
	};

	std::unique_ptr<Uniform> constructUniform(GLenum type);

	struct ConstantInfo
	{
		GLint location;
		GLenum type;
	};

	struct ShaderProgram
	{
		~ShaderProgram();

		void attach(GLenum type, const std::string& source, const std::string& fileName);
		void transformFeedback(const std::vector<std::string> &transformFeedbackVaryings, GLenum bufferMode);
		GLuint link();
		void create();
		void destroy();
		void set() const;

		GLint attrib(const GLchar* name) const;

		GLint uniformLocation(const GLchar* name) const;
		ConstantInfo uniformInfo(const GLchar* name) const;

		GLuint program = 0;
	};

	class ShaderProgramCompiler
	{
	public:
		void compile(ShaderProgram& program);
		void attach(GLenum type, const std::string& source, const std::string& file, int versionTag);
		void transformFeedback(const std::vector<std::string> &transformFeedbackVaryings, GLenum bufferMode);
		void serialize(serializer& os);

		std::vector<ConstantInfo> attributes;
		std::vector<ConstantInfo> uniforms;

	private:
		std::map<GLenum, std::tuple<std::string, std::string>> shaders;
		std::vector<std::string> transformFeedbackVaryings;
		GLenum bufferMode;
	};

}

#endif