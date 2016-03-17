##### Add include paths #####
include_directories(${PROJECT_SOURCE_DIR}/src/elfhacks)
include_directories(${PROJECT_SOURCE_DIR}/src/glinject)

SET(freetype-gl_USE_VAO ON CACHE BOOL "Use a VAO to render a vertex_buffer instance (required for forward compatible OpenGL 3.0 contexts)" FORCE)
SET(freetype-gl_BUILD_DEMOS OFF CACHE BOOL "Build the freetype-gl example programs" FORCE)
SET(freetype-gl_BUILD_APIDOC OFF CACHE BOOL "Build the freetype-gl API documentation" FORCE)
SET(freetype-gl_BUILD_HARFBUZZ OFF CACHE BOOL "Build the freetype-gl harfbuzz support (experimental)" FORCE)
SET(freetype-gl_BUILD_MAKEFONT OFF CACHE BOOL "Build the makefont tool" FORCE)

##### Descend into components #####
add_subdirectory(elfhacks)
add_subdirectory(glinject)
add_subdirectory(freetype-gl-glxosd)

##### Install scripts #####
INSTALL(DIRECTORY glxosd
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/glxosd/
        PATTERN "conf/*" EXCLUDE)

INSTALL(DIRECTORY freetype-gl-glxosd/freetype-gl/shaders
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/glxosd/freetype-gl-glxosd/freetype-gl/
        )
