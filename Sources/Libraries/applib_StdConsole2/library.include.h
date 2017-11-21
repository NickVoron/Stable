#pragma once

#ifdef DEFINE_SDK_LIB
#pragma DEFINE_SDK_LIB(applib_StdConsole2)
#endif

#if defined(__ANDROID__)
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <jni.h>
namespace applib
{
	void main(AAssetManager* manager);
}
#endif

#if defined(__APPLE__)
namespace applib
{
	void main();
}
#endif