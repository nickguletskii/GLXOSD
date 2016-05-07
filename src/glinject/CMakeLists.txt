include(GNUInstallDirs)
add_library(glinject SHARED glinject.c glx_events.c x_events.c)
add_dependencies(glinject elfhacks)
target_link_libraries(glinject dl rt luajit elfhacks m)
set_target_properties(glinject PROPERTIES LIBRARY_OUTPUT_NAME glxosd-glinject)
set_target_properties(glinject PROPERTIES VERSION ${GLXOSD_VERSION_STRING}
                                          SOVERSION ${GLXOSD_VERSION_MAJOR})
set_target_properties(glinject PROPERTIES LINK_FLAGS "-Wl,--version-script=${CMAKE_SOURCE_DIR}/src/glinject/glinject.version")
INSTALL(TARGETS glinject
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/glxosd
)
