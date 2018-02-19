file(GLOB_RECURSE allSourceFiles
    "*.h"
    "*.hpp"
    "*.hxx"
    "*.cpp"
    "*.c"
    "*.cxx"
)

get_filename_component(LibraryId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" LibraryId ${LibraryId})
add_library(${LibraryId} ${allSourceFiles})
set_target_properties(${LibraryId} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${SharedTecINT})