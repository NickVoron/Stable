// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "stuff/library.include.h"
#include "glew/library.include.h"

#ifndef EGL_PLATFORM_ANGLE_DEVICE_TYPE_NULL_ANGLE
#define EGL_PLATFORM_ANGLE_DEVICE_TYPE_NULL_ANGLE 0x6AC0
#endif

struct EGLPlatformParameters
{
    EGLint renderer;
    EGLint majorVersion;
    EGLint minorVersion;
    EGLint deviceType;
    EGLint presentPath;

    EGLPlatformParameters();
    explicit EGLPlatformParameters(EGLint renderer);
    EGLPlatformParameters(EGLint renderer, EGLint majorVersion, EGLint minorVersion, EGLint deviceType);
    EGLPlatformParameters(EGLint renderer, EGLint majorVersion, EGLint minorVersion, EGLint deviceType, EGLint presentPath);
};

bool operator<(const EGLPlatformParameters &a, const EGLPlatformParameters &b);
bool operator==(const EGLPlatformParameters &a, const EGLPlatformParameters &b);

struct EGLContextConfig
{
	EGLint redBits = EGL_DONT_CARE;
	EGLint greenBits = EGL_DONT_CARE;
	EGLint blueBits = EGL_DONT_CARE;
	EGLint alphaBits = EGL_DONT_CARE;
	EGLint depthBits = EGL_DONT_CARE;
	EGLint stencilBits = EGL_DONT_CARE;
	EGLBoolean multisample = EGL_FALSE;
	EGLBoolean debug = EGL_FALSE;
	EGLBoolean noError = EGL_FALSE;
	EGLint swapInterval = 0;
};

class EGLWindow : Base::NonCopyable
{
public:
    EGLWindow(EGLint glesMajorVersion, EGLint glesMinorVersion, const EGLPlatformParameters &platform);
    ~EGLWindow();

    static EGLBoolean FindEGLConfig(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *config);

    bool init(EGLNativeWindowType window, void* display);
    void release();
    bool initialized() const;
	void swap();

  private:
    EGLConfig config;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;

    EGLint clientMajorVersion;
    EGLint clientMinorVersion;
    EGLPlatformParameters platform;
	EGLContextConfig ctxconfig;
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