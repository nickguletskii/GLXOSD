include(CMakeParseArguments)
include(ExternalProject)

set(INSTALL_COMMAND
		make
		CFLAGS=-fPIC
		BUILDMODE=static
		Q=
		PREFIX=${CMAKE_CURRENT_BINARY_DIR}/luajit-2.0.4
		install)

ExternalProject_Add(project_luajit
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/luajit-2.0.4
	URL ${PROJECT_SOURCE_DIR}/third-party/LuaJIT-2.0.4.tar.gz
	CONFIGURE_COMMAND ""
	BUILD_IN_SOURCE 1
	BUILD_COMMAND ""
	INSTALL_COMMAND "${INSTALL_COMMAND}")

ExternalProject_Get_Property(project_luajit install_dir)
include_directories(${install_dir}/include/luajit-2.0)

add_library(luajit STATIC IMPORTED)
set_property(TARGET luajit PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libluajit-5.1.a)
add_dependencies(luajit project_luajit)
