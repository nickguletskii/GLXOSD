include(GNUInstallDirs)
set(GLXOSD_SOURCE_FILES GLXOSD.cpp GLXOSDLoader.cpp OSDInstance.cpp ConfigurationManager.cpp Utils.cpp GLAttribState.cpp FontRenderer.cpp ShaderProgram.cpp VertexBuffer.cpp Colour.cpp)
add_library(glxosd SHARED ${GLXOSD_SOURCE_FILES})
add_dependencies(glxosd glinject)
target_link_libraries(glxosd dl rt ${FREETYPE_LIBRARIES} ${OPENGL_LIBRARIES} ${FONTCONFIG_LIBRARY} ${SENSORS_LIBRARIES} ${Boost_LIBRARIES} ${OUTPUT_DIR}/libglxosd-glinject.so)
set_target_properties(glxosd PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR} LIBRARY_OUTPUT_NAME glxosd)
set_target_properties(glxosd PROPERTIES VERSION ${GLXOSD_VERSION_STRING}
                                          SOVERSION ${GLXOSD_VERSION_MAJOR})
INSTALL(TARGETS glxosd
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/glxosd
)
