include(GNUInstallDirs)

INCLUDE_DIRECTORIES(
	freetype-gl
)

set(FREETYPE_GL_SYMBOLS
	mat4_set_identity
	mat4_set_orthographic
	mat4_set_translation
	text_buffer_align
	text_buffer_clear
	text_buffer_delete
	text_buffer_get_bounds
	text_buffer_new_with_program
	text_buffer_printf
	text_buffer_render
	texture_atlas_upload
	texture_font_delete
	texture_font_new_from_file
	vector_delete
	font_manager_delete
	vertex_buffer_delete
)

SET(FREETYPE_GL_SYMBOL_WHITELIST "")
FOREACH(FREETYPE_GL_SYMBOL ${FREETYPE_GL_SYMBOLS})
	LIST(APPEND FREETYPE_GL_SYMBOL_WHITELIST "\n\t\tglxosd_ftgl_${FREETYPE_GL_SYMBOL}")
ENDFOREACH(FREETYPE_GL_SYMBOL)

configure_file(
	${CMAKE_SOURCE_DIR}/src/freetype-gl-glxosd/freetype-gl.version.in
	${CMAKE_CURRENT_BINARY_DIR}/src/freetype-gl-glxosd/freetype-gl.version
	@ONLY
)

SET(FREETYPE_GL_SRC
	freetype-gl/distance-field.c
	freetype-gl/distance-field.h
	freetype-gl/edtaa3func.c
	freetype-gl/edtaa3func.h
	freetype-gl/font-manager.c
	freetype-gl/font-manager.h
	freetype-gl/freetype-gl.h
	freetype-gl/markup.h
	freetype-gl/mat4.c
	freetype-gl/mat4.h
	freetype-gl/opengl.h
	freetype-gl/platform.c
	freetype-gl/platform.h
	freetype-gl/shader.c
	freetype-gl/shader.h
	freetype-gl/text-buffer.c
	freetype-gl/text-buffer.h
	freetype-gl/texture-atlas.c
	freetype-gl/texture-atlas.h
	freetype-gl/texture-font.c
	freetype-gl/texture-font.h
	freetype-gl/utf8-utils.c
	freetype-gl/utf8-utils.h
	freetype-gl/vec234.h
	freetype-gl/vector.c
	freetype-gl/vector.h
	freetype-gl/vertex-attribute.c
	freetype-gl/vertex-attribute.h
	freetype-gl/vertex-buffer.c
	freetype-gl/vertex-buffer.h
	gl-dynamic-loader.c
	gl-dynamic-loader.h
)

ADD_DEFINITIONS( -DFREETYPE_GL_USE_VAO )
set(FREETYPE-GL_VERSION_MAJOR 1)
set(FREETYPE-GL_VERSION_MINOR 1)
set(FREETYPE-GL_VERSION_PATCH 0)
set(FREETYPE-GL_VERSION_STRING ${FREETYPE-GL_VERSION_MAJOR}.${FREETYPE-GL_VERSION_MINOR}.${FREETYPE-GL_VERSION_PATCH})
add_library(freetype-gl-shared SHARED ${FREETYPE_GL_SRC})

foreach(FREETYPE_GL_SYMBOL ${FREETYPE_GL_SYMBOLS})
	target_compile_definitions(freetype-gl-shared PRIVATE
		${FREETYPE_GL_SYMBOL}=glxosd_ftgl_${FREETYPE_GL_SYMBOL}
	)
endforeach(FREETYPE_GL_SYMBOL)

target_link_libraries(freetype-gl-shared dl rt ${FONTCONFIG_LIBRARY} glew freetype)
set_target_properties(freetype-gl-shared PROPERTIES LIBRARY_OUTPUT_NAME glxosd-freetype-gl)
set_target_properties(freetype-gl-shared PROPERTIES VERSION ${FREETYPE-GL_VERSION_STRING}
        SOVERSION ${FREETYPE-GL_VERSION_MAJOR})
set_target_properties(freetype-gl-shared PROPERTIES LINKER_LANGUAGE C)
set_target_properties(freetype-gl-shared PROPERTIES LINK_FLAGS "-Wl,--version-script=${CMAKE_CURRENT_BINARY_DIR}/src/freetype-gl-glxosd/freetype-gl.version")
INSTALL(TARGETS freetype-gl-shared
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/glxosd
)
