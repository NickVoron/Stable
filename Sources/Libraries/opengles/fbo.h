#pragma once

#include "glew/library.include.h"

#include <functional>
#include <memory>

struct TexInfo
{
	TexInfo();
	void defaults();
	void clear();
	void bind() const;
	void setup(GLsizei width, GLsizei height, GLenum format);
	void setup(GLsizei width, GLsizei height, GLenum format, const void* pixels);
	unsigned int memsize() const;

	GLuint fbo = 0;
	GLuint tex = 0;
	GLsizei width = 0;
	GLsizei height = 0;
	GLenum format = GL_UNSIGNED_BYTE;
};

struct FBO
{
	~FBO();
	void setup(GLsizei width, GLsizei height, GLenum format);
	void render(const std::function<void ()>& function);
	void clear();
	

	std::unique_ptr<TexInfo> info;
};
