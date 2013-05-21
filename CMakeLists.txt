cmake_minimum_required (VERSION 2.6)
project (glxosd)
get_filename_component(OUTPUT_DIR "bin/" ABSOLUTE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")
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
##### Link to local libraries #####
link_directories(${OUTPUT_DIR})
##### Add include paths #####
include_directories(${PROJECT_SOURCE_DIR}/src/elfhacks)
include_directories(${PROJECT_SOURCE_DIR}/src/ftgl)
include_directories(${PROJECT_SOURCE_DIR}/src/glinject)
include_directories(${PROJECT_SOURCE_DIR}/src/glxosd)
##### Add subdirectories #####
add_subdirectory(src)
add_custom_command(OUTPUT glxosdlauncher COMMAND ./write_preload.sh DEPENDS glxosd COMMENT "Creating the startup script...")
add_custom_target(preloader ALL DEPENDS glxosdlauncher)
install(PROGRAMS glxosd DESTINATION bin)