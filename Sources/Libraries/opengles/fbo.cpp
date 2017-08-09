// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "fbo.h"
#include "opengl/library.include.h"
#include "defaultLogs/library.include.h"

struct TextureID
{
	GLsizei w;
	GLsizei h;
	GLenum format;

	bool operator<(const TextureID& id) const { return std::tie(w, h, format) < std::tie(id.w, id.h, id.format); }
};

struct TexturesCacheList : public std::list< std::unique_ptr<TexInfo> >
{

};

struct TexturesCache : public std::map<TextureID, TexturesCacheList>
{
	std::unique_ptr<TexInfo> get(GLsizei width, GLsizei height, GLenum format)
	{











		auto res = new TexInfo();
		res->setup(width, height, format);
		return std::unique_ptr<TexInfo>(res);
	}

	void add(std::unique_ptr<TexInfo>& info)
	{
	
	
	}
};

TexInfo::TexInfo()
{
	defaults();
}

void TexInfo::defaults()
{
	fbo = 0;
	tex = 0;
	width = 0;
	height = 0;
	format = GL_UNSIGNED_BYTE;
}

void TexInfo::clear()
{
	if (tex) GL_CALL(glDeleteTextures(1, &tex));
	if (fbo) GL_CALL(glDeleteFramebuffers(1, &fbo));

	defaults();
}

void TexInfo::setup(GLsizei width_, GLsizei height_, GLenum format_)
{
	if (width != width_ || height != height_ || format != format_)
	{
		clear();

		width = width_;
		height = height_;
		format = format_;
		
		GL_CALL(glGenFramebuffers(1, &fbo));
		GL_CALL(glGenTextures(1, &tex));

		GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

		GL_CALL(glBindTexture(GL_TEXTURE_2D, tex));
		GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, format, NULL));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_CALL(glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0));

		switch (glCheckFramebufferStatus(GL_FRAMEBUFFER))
		{
		case GL_FRAMEBUFFER_COMPLETE: break;
		case GL_FRAMEBUFFER_UNSUPPORTED: LOG_ERROR("FBO unsupported"); break;
		default: LOG_ERROR("FBO failed"); break;
		}
	}
}

void TexInfo::setup(GLsizei width_, GLsizei height_, GLenum format_, const void* pixels)
{
	if (width != width_ || height != height_ || format != format_)
	{
		clear();

		width = width_;
		height = height_;
		format = format_;

		GL_CALL(glGenTextures(1, &tex));

		GL_CALL(glBindTexture(GL_TEXTURE_2D, tex));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
		GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, format, pixels));
	}
}

unsigned int TexInfo::memsize() const
{
	return width * height;
}

void TexInfo::bind() const
{
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
}

TexturesCache texturesCache;

FBO::~FBO()
{
	clear();
}

void FBO::clear()
{
	texturesCache.add(info);
}			   

void FBO::setup(GLsizei width, GLsizei height, GLenum format)
{
	info = texturesCache.get(width, height, format);
}



void FBO::render(const std::function<void()>& function)
{
	info->bind();
	GL_CALL(glViewport(0, 0, info->width, info->height));
	GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
	function();
}




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