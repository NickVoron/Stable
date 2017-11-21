#include "opengl.h"

#include "gapi/library.include.h"



namespace opengl
{
#if ENABLE_OPENGL

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

	OpenGL::~OpenGL()
	{
		release();
	}

	HGLRC OpenGL::init(HDC hdc)
	{
		release();

		PIXELFORMATDESCRIPTOR pfd;
		int format;

		memset( &pfd, 0, sizeof( pfd ));
		pfd.nSize = sizeof( pfd );


		pfd.nVersion = 1; // версия всегда равна 1 (MSDN)
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;

		format = ChoosePixelFormat( hdc, &pfd );
		SetPixelFormat( hdc, format, &pfd );

		HGLRC hglrc = wglCreateContext( hdc );
		GL_CALL(wglMakeCurrent(hdc, hglrc));
		
		wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>( wglGetProcAddress( "wglCreateContextAttribsARB" ));

		wglMakeCurrent( 0, 0 );
		wglDeleteContext( hglrc );

	 	int attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 5,
			WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		hglrc = wglCreateContextAttribsARB( hdc, 0, attribs );
		GL_CALL(wglMakeCurrent( hdc, hglrc ));
		
		if( glewInit() != GLEW_OK )
		{		
			// обработка ошибки
			LOG_ERROR("not glew");
		}

		opengl::check(SOURCE_LOCATION);
		if( !GLEW_VERSION_4_5 )
		{	
			// обработка ошибки
			LOG_ERROR("not OpenGL 4.2");
		}							 
		opengl::check(SOURCE_LOCATION);

		ctx = hglrc;
		return hglrc;
	}

	HGLRC OpenGL::context() const
	{
		return ctx;
	}

	void OpenGL::release()
	{
		if (ctx)
		{
			wglDeleteContext(ctx);
			ctx = 0;
		}
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

