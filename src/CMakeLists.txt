##### Add include paths #####
include_directories(${PROJECT_SOURCE_DIR}/src/elfhacks)
include_directories(${PROJECT_SOURCE_DIR}/src/glinject)


SET(freetype-gl_USE_VAO TRUE)
SET(freetype-gl_BUILD_DEMOS FALSE)
SET(freetype-gl_BUILD_APIDOC FALSE)
SET(freetype-gl_BUILD_HARFBUZZ FALSE)
SET(freetype-gl_BUILD_MAKEFONT FALSE)

##### Descend into components #####
add_subdirectory(elfhacks)
add_subdirectory(glinject)
add_subdirectory(freetype-gl-glxosd)

##### Install scripts #####
INSTALL(DIRECTORY glxosd
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/glxosd/
        PATTERN "conf/*" EXCLUDE)
