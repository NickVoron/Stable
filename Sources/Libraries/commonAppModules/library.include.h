#pragma once

#include "locale.h"
#include "logs.h"
#include "qtlog.h"
#include "logsUpdater.h"
#include "resourceManager.h"
#include "streamer.h"
#include "settings.h"
#include "processTerminator.h"
#include "autorun.h"
#include "componentModel.h"
#include "crtMemDebugger.h" 
#include "jni.h"

#ifdef WIN32
#include "componentModelHelpers.h"
#include "camera.h"
#include "cameraCreator.h"
#include "winInput.h"
#include "commandProcessor.h"
#include "console.h"
#include "debugDrawing.h"
#include "screenshots.h"
#include "window.h"
#include "winInput.h"
#include "workDir.h"
#include "freeimage.h"
#include "timer.h"
#include "screenLog.h"
#include "imgui.h"
#include "imguiApp.h"
#include "uniguiFlusher.h"
#include "profiler.h"
#include "lightSources.h"
#include "screenMouseHelper.h"
//#include "renderTargetsManager.h"
#include "postEffects.h"
#include "manipulators3d.h"
#include "openal.h"
#include "dragNDrop.h"
#include "windowParams.h"
//#include "renderPipeline.h"
#include "udtnet.h"
#include "utpnet.h"
#include "fbx.h"

#include "d3d9/direct3d9.h"
#include "d3d11/direct3d11.h"
#include "opengl/opengl.h"
#include "opengles/opengles.h"
#endif


#ifdef DEFINE_SDK_LIB
#pragma DEFINE_SDK_LIB(commonAppModules)
#endif

