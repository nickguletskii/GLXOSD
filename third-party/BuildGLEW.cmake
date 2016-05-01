include(CMakeParseArguments)
include(ExternalProject)

set(INSTALL_COMMAND
	make
	CFLAGS=-fPIC
	LIBDIR=${CMAKE_CURRENT_BINARY_DIR}/glew-1.13.0/lib
	GLEW_DEST=${CMAKE_CURRENT_BINARY_DIR}/glew-1.13.0
	install)

ExternalProject_Add(project_glew
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/glew-1.13.0
	URL ${PROJECT_SOURCE_DIR}/third-party/glew-1.13.0.tgz
	CONFIGURE_COMMAND ""
	BUILD_IN_SOURCE 1
	BUILD_COMMAND ""
	INSTALL_COMMAND "${INSTALL_COMMAND}")

ExternalProject_Get_Property(project_glew install_dir)
include_directories(${install_dir}/include/)
add_library(glew STATIC IMPORTED)
set_property(TARGET glew PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libGLEW.a)
add_dependencies(glew project_glew)
