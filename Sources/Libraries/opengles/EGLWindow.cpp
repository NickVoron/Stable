// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "EGLWindow.h"

EGLPlatformParameters::EGLPlatformParameters()
    : renderer(EGL_PLATFORM_ANGLE_TYPE_DEFAULT_ANGLE),
      majorVersion(EGL_DONT_CARE),
      minorVersion(EGL_DONT_CARE),
      deviceType(EGL_DONT_CARE),
      presentPath(EGL_DONT_CARE)
{
}

EGLPlatformParameters::EGLPlatformParameters(EGLint renderer)
    : renderer(renderer),
      majorVersion(EGL_DONT_CARE),
      minorVersion(EGL_DONT_CARE),
      deviceType(EGL_DONT_CARE),
      presentPath(EGL_DONT_CARE)
{
    if (renderer == EGL_PLATFORM_ANGLE_TYPE_D3D9_ANGLE ||
        renderer == EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE)
    {
        deviceType = EGL_PLATFORM_ANGLE_DEVICE_TYPE_HARDWARE_ANGLE;
    }
}

EGLPlatformParameters::EGLPlatformParameters(EGLint renderer, EGLint majorVersion, EGLint minorVersion, EGLint useWarp)
    : renderer(renderer),
      majorVersion(majorVersion),
      minorVersion(minorVersion),
      deviceType(useWarp),
      presentPath(EGL_DONT_CARE)
{
}

EGLPlatformParameters::EGLPlatformParameters(EGLint renderer, EGLint majorVersion, EGLint minorVersion, EGLint useWarp, EGLint presentPath)
    : renderer(renderer),
      majorVersion(majorVersion),
      minorVersion(minorVersion),
      deviceType(useWarp),
      presentPath(presentPath)
{
}

bool operator<(const EGLPlatformParameters &a, const EGLPlatformParameters &b)
{
	return	
		std::tie(a.renderer, a.majorVersion, a.minorVersion, a.deviceType, a.presentPath) <
		std::tie(b.renderer, b.majorVersion, b.minorVersion, b.deviceType, b.presentPath);
}

bool operator==(const EGLPlatformParameters &a, const EGLPlatformParameters &b)
{
    return (a.renderer == b.renderer) && (a.majorVersion == b.majorVersion) &&
           (a.minorVersion == b.minorVersion) && (a.deviceType == b.deviceType) &&
           (a.presentPath == b.presentPath);
}

EGLWindow::EGLWindow(EGLint glesMajorVersion, EGLint glesMinorVersion, const EGLPlatformParameters &platform)
    : display(EGL_NO_DISPLAY),
      surface(EGL_NO_SURFACE),
      context(EGL_NO_CONTEXT),
      clientMajorVersion(glesMajorVersion),
      clientMinorVersion(glesMinorVersion),
      platform(platform)
{
}

EGLWindow::~EGLWindow()
{
    release();
}

void EGLWindow::swap()
{
    eglSwapBuffers(display, surface);
}


bool EGLWindow::init(EGLNativeWindowType window, void* display)
{
    PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT = reinterpret_cast<PFNEGLGETPLATFORMDISPLAYEXTPROC>(eglGetProcAddress("eglGetPlatformDisplayEXT"));
    if (!eglGetPlatformDisplayEXT)
    {
        return false;
    }

    std::vector<EGLint> displayAttributes;
    displayAttributes.push_back(EGL_PLATFORM_ANGLE_TYPE_ANGLE);
    displayAttributes.push_back(platform.renderer);
    displayAttributes.push_back(EGL_PLATFORM_ANGLE_MAX_VERSION_MAJOR_ANGLE);
    displayAttributes.push_back(platform.majorVersion);
    displayAttributes.push_back(EGL_PLATFORM_ANGLE_MAX_VERSION_MINOR_ANGLE);
    displayAttributes.push_back(platform.minorVersion);

    if (platform.deviceType != EGL_DONT_CARE)
    {
        displayAttributes.push_back(EGL_PLATFORM_ANGLE_DEVICE_TYPE_ANGLE);
        displayAttributes.push_back(platform.deviceType);
    }

    if (platform.presentPath != EGL_DONT_CARE)
    {
        const char *extensionString = static_cast<const char *>(eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS));
        if (strstr(extensionString, "EGL_ANGLE_experimental_present_path") == nullptr)
        {
            release();
            return false;
        }

        displayAttributes.push_back(EGL_EXPERIMENTAL_PRESENT_PATH_ANGLE);
        displayAttributes.push_back(platform.presentPath);
    }
    displayAttributes.push_back(EGL_NONE);

    display = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, display, &displayAttributes[0]);
    if (display == EGL_NO_DISPLAY)
    {
		release();
        return false;
    }

    EGLint majorVersion, minorVersion;
    if (eglInitialize(display, &majorVersion, &minorVersion) == EGL_FALSE)
    {
		release();
        return false;
    }

    const char *displayExtensions = eglQueryString(display, EGL_EXTENSIONS);

    
    bool hasKHRCreateContext = strstr(displayExtensions, "EGL_KHR_create_context") != nullptr;
    if (majorVersion != 2 && minorVersion != 0 && !hasKHRCreateContext)
    {
		release();
        return false;
    }

    eglBindAPI(EGL_OPENGL_ES_API);
    if (eglGetError() != EGL_SUCCESS)
    {
		release();
        return false;
    }

    const EGLint configAttributes[] =
    {
        EGL_RED_SIZE,       ctxconfig.redBits     ,
        EGL_GREEN_SIZE,     ctxconfig.greenBits   ,
        EGL_BLUE_SIZE,      ctxconfig.blueBits    ,
        EGL_ALPHA_SIZE,     ctxconfig.alphaBits   ,
        EGL_DEPTH_SIZE,     ctxconfig.depthBits   ,
        EGL_STENCIL_SIZE,   ctxconfig.stencilBits ,
        EGL_SAMPLE_BUFFERS, ctxconfig.multisample ? 1 : 0,
        EGL_NONE
    };

    EGLint configCount;
    if (!eglChooseConfig(display, configAttributes, &config, 1, &configCount) || (configCount != 1))
    {
		release();
        return false;
    }

    eglGetConfigAttrib(display, config, EGL_RED_SIZE,      &ctxconfig.redBits);
    eglGetConfigAttrib(display, config, EGL_GREEN_SIZE,    &ctxconfig.greenBits);
    eglGetConfigAttrib(display, config, EGL_BLUE_SIZE,     &ctxconfig.blueBits);
    eglGetConfigAttrib(display, config, EGL_ALPHA_SIZE,    &ctxconfig.alphaBits);
    eglGetConfigAttrib(display, config, EGL_DEPTH_SIZE,    &ctxconfig.depthBits);
    eglGetConfigAttrib(display, config, EGL_STENCIL_SIZE,  &ctxconfig.stencilBits);

    std::vector<EGLint> surfaceAttributes;
    if (strstr(displayExtensions, "EGL_NV_post_sub_buffer") != nullptr)
    {
        surfaceAttributes.push_back(EGL_POST_SUB_BUFFER_SUPPORTED_NV);
        surfaceAttributes.push_back(EGL_TRUE);
    }

    surfaceAttributes.push_back(EGL_NONE);

    surface = eglCreateWindowSurface(display, config, window, &surfaceAttributes[0]);
    if (eglGetError() != EGL_SUCCESS)
    {
		release();
        return false;
    }
    ENFORCE(surface != EGL_NO_SURFACE);

    std::vector<EGLint> contextAttributes;
    if (hasKHRCreateContext)
    {
        contextAttributes.push_back(EGL_CONTEXT_MAJOR_VERSION_KHR);
        contextAttributes.push_back(clientMajorVersion);

        contextAttributes.push_back(EGL_CONTEXT_MINOR_VERSION_KHR);
        contextAttributes.push_back(clientMinorVersion);

        contextAttributes.push_back(EGL_CONTEXT_OPENGL_DEBUG);
        contextAttributes.push_back(ctxconfig.debug);

        bool hasKHRCreateContextNoError = strstr(displayExtensions, "EGL_KHR_create_context_no_error") != nullptr;

        contextAttributes.push_back(EGL_CONTEXT_OPENGL_NO_ERROR_KHR);
        contextAttributes.push_back(ctxconfig.noError);
    }
    contextAttributes.push_back(EGL_NONE);

    context = eglCreateContext(display, config, nullptr, &contextAttributes[0]);
    if (eglGetError() != EGL_SUCCESS)
    {
		release();
        return false;
    }

    eglMakeCurrent(display, surface, surface, context);
    if (eglGetError() != EGL_SUCCESS)
    {
		release();
        return false;
    }

    if (ctxconfig.swapInterval != -1)
    {
        eglSwapInterval(display, ctxconfig.swapInterval);
    }

    return true;
}

void EGLWindow::release()
{
    if (surface != EGL_NO_SURFACE)
    {
        assert(display != EGL_NO_DISPLAY);
        eglDestroySurface(display, surface);
        surface = EGL_NO_SURFACE;
    }

    if (context != EGL_NO_CONTEXT)
    {
        assert(display != EGL_NO_DISPLAY);
        eglDestroyContext(display, context);
        context = EGL_NO_CONTEXT;
    }

    if (display != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglTerminate(display);
        display = EGL_NO_DISPLAY;
    }
}

bool EGLWindow::initialized() const
{
    return surface != EGL_NO_SURFACE &&
           context != EGL_NO_CONTEXT &&
           display != EGL_NO_DISPLAY;
}

EGLBoolean EGLWindow::FindEGLConfig(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *config)
{
    EGLint numConfigs = 0;
    eglGetConfigs(dpy, nullptr, 0, &numConfigs);
    std::vector<EGLConfig> allConfigs(numConfigs);
    eglGetConfigs(dpy, allConfigs.data(), static_cast<EGLint>(allConfigs.size()), &numConfigs);

    for (size_t i = 0; i < allConfigs.size(); i++)
    {
        bool matchFound = true;
        for (const EGLint *curAttrib = attrib_list; curAttrib[0] != EGL_NONE; curAttrib += 2)
        {
            EGLint actualValue = EGL_DONT_CARE;
            eglGetConfigAttrib(dpy, allConfigs[i], curAttrib[0], &actualValue);
            if (curAttrib[1] != actualValue)
            {
                matchFound = false;
                break;
            }
        }

        if (matchFound)
        {
            *config = allConfigs[i];
            return EGL_TRUE;
        }
    }

    return EGL_FALSE;
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