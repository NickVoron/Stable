// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "opengl.h"

#include "gapi/library.include.h"



namespace opengl
{
#if ENABLE_OPENGL

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

	HGLRC init(HDC hdc)
	{
		PIXELFORMATDESCRIPTOR pfd;
		int format;

		memset( &pfd, 0, sizeof( pfd ));
		pfd.nSize = sizeof( pfd );


		pfd.nVersion = 1; 
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;

		format = ChoosePixelFormat( hdc, &pfd );
		SetPixelFormat( hdc, format, &pfd );

		HGLRC hglrc = wglCreateContext( hdc );
		wglMakeCurrent( hdc, hglrc );
		opengl::check();

		wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>( wglGetProcAddress( "wglCreateContextAttribsARB" ));

		wglMakeCurrent( 0, 0 );
		wglDeleteContext( hglrc );

	 	int attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 2,
			WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		hglrc = wglCreateContextAttribsARB( hdc, 0, attribs );
		wglMakeCurrent( hdc, hglrc );
		opengl::check();

		if( glewInit() != GLEW_OK )
		{		
			
			LOG_ERROR("not glew");
		}

		if( !GLEW_VERSION_4_2 )
		{	
			
			LOG_ERROR("not OpenGL 4.2");
		}

		opengl::check();

		return hglrc;
	}


	void release()
	{

	}
#endif

	Viewport::Viewport()
	{

	}

	void Viewport::init(int w, int h) 
	{
		size = nm::index2(w, h); 
	}

	int Viewport::width() const { return size.x; }
	int Viewport::height() const { return size.y; }
	void Viewport::set() const {  }
	int Viewport::deviceIndex() const { return 0; }
}





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