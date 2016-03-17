cmake_minimum_required (VERSION 2.6)
project (glxosd)
include(GNUInstallDirs)

##### Parameters #####
if (EXISTS "/etc/debian_version")
    SET(INSTALLATION_SUFFIX_32 "/lib/i386-linux-gnu/")
    SET(INSTALLATION_SUFFIX_64 "/lib/x86_64-linux-gnu/")
else()
    SET(INSTALLATION_SUFFIX_32 "/lib/i386-linux-gnu/")
    SET(INSTALLATION_SUFFIX_64 "/lib/x86_64-linux-gnu/")
endif()


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")

add_definitions(-DLUA_SOURCECODE_PATH="${CMAKE_INSTALL_FULL_DATAROOTDIR}/glxosd/")
##### Compiler #####
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11")
SET(CMAKE_C_LINK_EXECUTABLE  "${CMAKE_C_LINK_EXECUTABLE}")
##### Set version #####
set(GLXOSD_VERSION_MAJOR 2)
set(GLXOSD_VERSION_MINOR 5)
set(GLXOSD_VERSION_PATCH 0)
set(GLXOSD_VERSION_STRING ${GLXOSD_VERSION_MAJOR}.${GLXOSD_VERSION_MINOR}.${GLXOSD_VERSION_PATCH})
##### Find the libraries #####
#Find OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)
if(NOT OPENGL_GLU_FOUND)
    message(ERROR " GLU not found!")
endif(NOT OPENGL_GLU_FOUND)
#Find Freetype
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
if(NOT FREETYPE_FOUND)
    message(ERROR " Freetype not found!")
endif(NOT FREETYPE_FOUND)
#Find FontConfig
find_package(FontConfig REQUIRED)
include_directories(${FONTCONFIG_INCLUDE_DIRS})
if(NOT FONTCONFIG_FOUND)
    message(ERROR " FontConfig not found!")
endif(NOT FONTCONFIG_FOUND)
#Find POSIX threads
find_package(Threads REQUIRED)
if(NOT Threads_FOUND)
    message(ERROR " POSIX threads not found!")
endif(NOT Threads_FOUND)
#Find X11
find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIRS})
if(NOT X11_FOUND)
    message(ERROR " X11 not found!")
endif(NOT X11_FOUND)
#Find luajit
find_package(LuaJIT)
include_directories(${LUAJIT_INCLUDE_DIR})
if(NOT LUAJIT_FOUND)
	message(ERROR " LuaJIT not found!")
	SET(LUAJIT_LIBRARIES "")
else(NOT LUAJIT_FOUND)
	message(INFO " luajit found!")
	add_definitions( -DFOUND_LUAJIT )
endif(NOT LUAJIT_FOUND)
##### Link to local libraries #####
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
##### Add subdirectories #####
add_subdirectory(src)

##### Install config #####
install(DIRECTORY src/glxosd/conf/
        DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/glxosd/)

##### Create launcher #####
message(INFO "BUILDING LAUNCHER!")
add_custom_target(glxosdlauncher COMMAND ${PROJECT_SOURCE_DIR}/write_preload.sh
--target-file "${PROJECT_BINARY_DIR}/glxosd"
--installation-prefix "${CMAKE_INSTALL_PREFIX}"
--installation-suffix-32 "${INSTALLATION_SUFFIX_32}"
--installation-suffix-64 "${INSTALLATION_SUFFIX_64}"
--cmake-install-full-datarootdir ${CMAKE_INSTALL_FULL_DATAROOTDIR}
COMMENT "Creating the startup script...")
add_dependencies(glinject glxosdlauncher)
install(PROGRAMS glxosd DESTINATION ${CMAKE_INSTALL_BINDIR})
