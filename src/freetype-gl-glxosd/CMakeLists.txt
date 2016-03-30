include(GNUInstallDirs)

SET(freetype-gl_USE_VAO TRUE)
SET(freetype-gl_BUILD_DEMOS FALSE)
SET(freetype-gl_BUILD_APIDOC FALSE)
SET(freetype-gl_BUILD_HARFBUZZ FALSE)
SET(freetype-gl_BUILD_MAKEFONT FALSE)
add_subdirectory(freetype-gl)

get_directory_property(FREETYPE_GL_SRC_LOCAL
        DIRECTORY freetype-gl
        DEFINITION FREETYPE_GL_SRC)

SET(FREETYPE_GL_SRC "")
LIST(APPEND FREETYPE_GL_SRC "gl-dynamic-loader.h")
LIST(APPEND FREETYPE_GL_SRC "gl-dynamic-loader.c")
FOREACH(f ${FREETYPE_GL_SRC_LOCAL})
    LIST(APPEND FREETYPE_GL_SRC "freetype-gl/${f}")
ENDFOREACH(f)

ADD_DEFINITIONS( -DFREETYPE_GL_USE_VAO )
set(FREETYPE-GL_VERSION_MAJOR 1)
set(FREETYPE-GL_VERSION_MINOR 0)
set(FREETYPE-GL_VERSION_PATCH 1)
set(FREETYPE-GL_VERSION_STRING ${FREETYPE-GL_VERSION_MAJOR}.${FREETYPE-GL_VERSION_MINOR}.${FREETYPE-GL_VERSION_PATCH})
add_library(freetype-gl-shared SHARED ${FREETYPE_GL_SRC})
target_link_libraries(freetype-gl-shared dl rt ${GLEW_LIBRARY} ${FONTCONFIG_LIBRARY} ${FREETYPE_LIBRARY})
set_target_properties(freetype-gl-shared PROPERTIES LIBRARY_OUTPUT_NAME glxosd-freetype-gl)
set_target_properties(freetype-gl-shared PROPERTIES VERSION ${FREETYPE-GL_VERSION_STRING}
        SOVERSION ${FREETYPE-GL_VERSION_MAJOR})
set_target_properties(freetype-gl-shared PROPERTIES LINKER_LANGUAGE C)
INSTALL(TARGETS freetype-gl-shared
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/glxosd
)