get_filename_component(LibraryId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" LibraryId ${LibraryId})
add_compile_options("-FIstdafx.h")
add_library(${LibraryId} 
	"componentModel.h"
    "componentModel.cpp"
    "scheme.h"
    "scheme.cpp"
    "autorun.h"
    "autorun.cpp"
    "resourceManager.h"
    "resourceManager.cpp"
    "crtMemDebugger.h"
    "crtMemDebugger.cpp"
    "processTerminator.h"
    "processTerminator.cpp"
    "utpnet.h"
    "utpnet.cpp"
    "timer.h"
    "timer.cpp"
    "jni.h"
    "jni.cpp"
	)
set_target_precompiled_header(${LibraryId} stdafx.h stdafx.cpp)
set_target_properties(${LibraryId} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "$ENV{SharedTecINT}/libs")