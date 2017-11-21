#pragma once

#if ENABLE_OPENGL_ES

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

#endif