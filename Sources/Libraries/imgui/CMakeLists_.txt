file(GLOB_RECURSE allSourceFiles
    "*.h"
    "*.hpp"
    "*.hxx"
    "*.cpp"
    "*.c"
    "*.cxx"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

get_filename_component(LibraryId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" LibraryId ${LibraryId})
add_compile_options("-FIstdafx.h")
add_library(${LibraryId} ${allSourceFiles})
set_target_precompiled_header(${LibraryId} stdafx.h stdafx.cpp)
set_target_properties(${LibraryId} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${SharedTecINT})