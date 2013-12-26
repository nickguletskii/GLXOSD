SET(GLXOSD_LIBSENSORS_SUPPORT_SOURCE_FILES LibsensorsSensorDataProvider.cpp)
add_library(glxosd-libsensors-support SHARED ${GLXOSD_LIBSENSORS_SUPPORT_SOURCE_FILES})
add_dependencies(glxosd-libsensors-support glxosd)
target_link_libraries(glxosd-libsensors-support dl rt ${SENSORS_LIBRARIES} ${OUTPUT_DIR}/libglxosd.so)
set_target_properties(glxosd-libsensors-support PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR} LIBRARY_OUTPUT_NAME glxosd-plugins-libsensors-support)
set_target_properties(glxosd-libsensors-support PROPERTIES VERSION ${GLXOSD_VERSION_STRING}
                                          SOVERSION ${GLXOSD_VERSION_MAJOR})
INSTALL(TARGETS glxosd-libsensors-support
  LIBRARY DESTINATION lib/${CMAKE_LIBRARY_ARCHITECTURE}/glxosd
)
