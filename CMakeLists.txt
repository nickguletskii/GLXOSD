cmake_minimum_required (VERSION 2.6)
project (glxosd)
include(GNUInstallDirs)

##### Parameters #####
SET(ATTEMPT_NVIDIA_LINK TRUE CACHE BOOL "If set to true, GLXOSD will link to the NVIDIA driver to display the GPU's temperature.")
SET(INSTALLATION_SUFFIX_32 "/lib/i386-linux-gnu/" CACHE STRING "The part of the path to the installation directory for 32 bit libraries that comes after the prefix.")
SET(INSTALLATION_SUFFIX_64 "/lib/x86_64-linux-gnu/" CACHE STRING "The part of the path to the installation directory for 64 bit libraries that comes after the prefix.")

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
#Find Sensors
find_package(Sensors)
include_directories(${SENSORS_INCLUDE_DIRS})
if(NOT SENSORS_FOUND)
	message(ERROR " libsensors not found!")
	SET(SENSORS_LIBRARIES "")
else(NOT SENSORS_FOUND)
	message(INFO " libsensors found!")
	add_definitions( -DFOUND_SENSORS )
endif(NOT SENSORS_FOUND)
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
#Find NVCTRL if no additional flag is set
if(ATTEMPT_NVIDIA_LINK)
	find_package(NVCTRL)
	include_directories(${NVCTRL_INCLUDE_DIRS})
	if(NOT NVCTRL_FOUND)
	message(INFO " NVCTRL not found!")
	SET(NVCTRL_LIBRARIES "")
	else(NOT NVCTRL_FOUND)
	message(INFO " NVCTRL found!")
		add_definitions( -DFOUND_NVCTRL )
	endif(NOT NVCTRL_FOUND)
endif(ATTEMPT_NVIDIA_LINK)
##### Link to local libraries #####
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
##### Add subdirectories #####
add_subdirectory(src)

##### Install fonts #####
install(FILES "fonts/CPMono_v07/CPMono_v07_Bold.ttf"  DESTINATION /usr/share/fonts/truetype/CPMono_v07)
install(SCRIPT "${CMAKE_SOURCE_DIR}/TriggerFontConfig.cmake")

##### Instal config #####
install(FILES conf/glxosd.conf DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/glxosd/)

##### Install src #####
INSTALL(DIRECTORY src/glxosd DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/glxosd/)
INSTALL(DIRECTORY src/LJIT2Khronos DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/glxosd/)
##### Create launcher #####
message(INFO "BUILDING LAUNCHER!")
add_custom_target(glxosdlauncher COMMAND ${PROJECT_SOURCE_DIR}/write_preload.sh
--target-file ${PROJECT_SOURCE_DIR}/glxosd
--installation-prefix ${CMAKE_INSTALL_PREFIX}
--installation-suffix-32 ${INSTALLATION_SUFFIX_32}
--installation-suffix-64 ${INSTALLATION_SUFFIX_64}
--cmake-install-full-datarootdir ${CMAKE_INSTALL_FULL_DATAROOTDIR}
COMMENT "Creating the startup script...")
add_dependencies(glinject glxosdlauncher)
install(PROGRAMS glxosd DESTINATION bin)

