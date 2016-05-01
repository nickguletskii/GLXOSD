include(CMakeParseArguments)
include(ExternalProject)

ExternalProject_Add(project_glew
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/glew-1.13.0
	SOURCE_DIR ${PROJECT_SOURCE_DIR}/third-party/glew-1.13.0/build/cmake/
	CMAKE_ARGS -D BUILD_UTILS=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/glew-1.13.0 -DCMAKE_C_FLAGS=-fPIC
)

ExternalProject_Get_Property(project_glew install_dir)
include_directories(${install_dir}/include)
add_library(glew STATIC IMPORTED)
set_property(TARGET glew PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libGLEW.a)
add_dependencies(glew project_glew)
