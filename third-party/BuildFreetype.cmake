include(CMakeParseArguments)
include(ExternalProject)

ExternalProject_Add(project_freetype
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/freetype-2.6
	URL ${PROJECT_SOURCE_DIR}/third-party/freetype-2.6.tar.gz
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/freetype-2.6 -DCMAKE_C_FLAGS=-fPIC
)

add_library(freetype STATIC IMPORTED)
set_property(TARGET freetype PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/freetype-2.6/lib/libfreetype.a)
add_dependencies(freetype project_freetype)
