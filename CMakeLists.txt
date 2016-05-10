cmake_minimum_required (VERSION 2.8)
project (glxosd)
include(GNUInstallDirs)
include(third-party/BuildLuaJIT.cmake)
include(third-party/BuildGLEW.cmake)
include(third-party/BuildFreetype.cmake)
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

##### Set version #####
set(GLXOSD_VERSION_MAJOR 3)
set(GLXOSD_VERSION_MINOR 2)
set(GLXOSD_VERSION_PATCH 0)
set(GLXOSD_VERSION_STRING ${GLXOSD_VERSION_MAJOR}.${GLXOSD_VERSION_MINOR}.${GLXOSD_VERSION_PATCH})

##### Compiler #####
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11")
SET(CMAKE_C_LINK_EXECUTABLE  "${CMAKE_C_LINK_EXECUTABLE}")

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
##### Link to local libraries #####
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
##### Add subdirectories #####
add_subdirectory(src)

##### Install config #####
install(DIRECTORY src/glxosd/conf/
        DESTINATION /etc/glxosd/)
##### Create launcher #####
configure_file(
	${PROJECT_SOURCE_DIR}/launcher_template.sh.in
	 ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/glxosd
	@ONLY
)
file(
	COPY ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/glxosd
	DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	FILE_PERMISSIONS
		OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_EXECUTE
		WORLD_READ WORLD_EXECUTE
)

install(FILES "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/glxosd" DESTINATION ${CMAKE_INSTALL_BINDIR})
