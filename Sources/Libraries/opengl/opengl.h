#pragma once

#include "gapi/library.include.h"
#include "viewports/library.include.h"

#include "shaders.h"
#include "debug.h"

		  
namespace opengl
{
#if ENABLE_OPENGL
	class OpenGL
	{
	public:
		~OpenGL();
		HGLRC init(HDC hdc);
		void release();
		HGLRC context() const;
	private:
		HGLRC ctx {0};
	};
	
#endif
				 
	class Viewport : public Viewports::Viewport
	{
	public:
		Viewport();
				
		void init(int width, int height);
		virtual int width() const;
		virtual int height() const;
		virtual void set() const;
		virtual int deviceIndex() const;
	private:
		nm::index2 size;
	};
}