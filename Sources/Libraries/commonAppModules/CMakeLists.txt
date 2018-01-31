get_filename_component(LibraryId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" LibraryId ${LibraryId})
add_compile_options("-FIstdafx.h")

set(allSourceFiles
    "componentModel.h"
    "componentModel.cpp"
    "scheme.h"
    "scheme.cpp"
    "autorun.h"
    "autorun.cpp"
    "resourceManager.h"
    "resourceManager.cpp"
    "utpnet.h"
    "utpnet.cpp"
    "timer.h"
    "timer.cpp"
)

if(USE_ANDROID)
set(allSourceFiles
	${allSourceFiles}
    "jni.h"
    "jni.cpp"
)
endif()

if(USE_WINDOWS)
set(allSourceFiles
	${allSourceFiles}
    "crtMemDebugger.h"
    "crtMemDebugger.cpp"
    "workDir.h"
    "workDir.cpp"
    "processTerminator.h"
    "processTerminator.cpp"
    "qtlog.h"
    "qtlog.cpp"
    "stdafx.h"
    "stdafx.cpp"
)
endif()

add_library( ${LibraryId} ${allSourceFiles} )

set_target_precompiled_header(${LibraryId} stdafx.h stdafx.cpp)
set_target_properties(${LibraryId} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${SharedTecINT})