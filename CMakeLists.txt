cmake_minimum_required (VERSION 2.6)
project (glxosd)

##### Parameters #####
SET(ATTEMPT_NVIDIA_LINK TRUE CACHE BOOL "If set to true, GLXOSD will link to the NVIDIA driver to display the GPU's temperature.")

get_filename_component(OUTPUT_DIR "bin/" ABSOLUTE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")
##### Set version #####
set(GLXOSD_VERSION_MAJOR 1)
set(GLXOSD_VERSION_MINOR 1)
set(GLXOSD_VERSION_PATCH 1)
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
#Find Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED)
    include_directories(${Boost_INCLUDE_DIRS})
if(NOT Boost_FOUND)
    message(ERROR " Boost not found!")                                                                                                                                                                                                                    
endif(NOT Boost_FOUND)
##### Link to local libraries #####
link_directories(${OUTPUT_DIR})
##### Add subdirectories #####
add_subdirectory(src)

##### Install fonts #####
install(FILES "fonts/CPMono_v07/CPMono_v07_Bold.otf"  DESTINATION /usr/share/fonts/opentype/CPMono_v07)
install(SCRIPT "${CMAKE_SOURCE_DIR}/TriggerFontConfig.cmake")

##### Instal config #####
install(FILES conf/glxosd.conf DESTINATION /etc)

##### Create launcher #####
message(INFO "BUILDING LAUNCHER!")
add_custom_target(glxosdlauncher COMMAND ${PROJECT_SOURCE_DIR}/write_preload.sh
--target-file ${PROJECT_SOURCE_DIR}/glxosd
--installation-prefix ${CMAKE_INSTALL_PREFIX} COMMENT "Creating the startup script...")
add_dependencies(glxosd glxosdlauncher)
install(PROGRAMS glxosd DESTINATION bin)

