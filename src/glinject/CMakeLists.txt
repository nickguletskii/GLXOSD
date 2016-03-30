include(GNUInstallDirs)
set(GLINJECT_VERSION_MAJOR 2)
set(GLINJECT_VERSION_MINOR 2)
set(GLINJECT_VERSION_PATCH 1)
set(GLINJECT_VERSION_STRING ${GLINJECT_VERSION_MAJOR}.${GLINJECT_VERSION_MINOR}.${GLINJECT_VERSION_PATCH})
add_library(glinject SHARED glinject.c glx_events.c x_events.c luajit_dynamic.c)
add_dependencies(glinject elfhacks)
target_link_libraries(glinject dl rt glxosd-elfhacks)
set_target_properties(glinject PROPERTIES LIBRARY_OUTPUT_NAME glxosd-glinject)
set_target_properties(glinject PROPERTIES VERSION ${GLINJECT_VERSION_STRING}
                                          SOVERSION ${GLINJECT_VERSION_MAJOR})
INSTALL(TARGETS glinject
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/glxosd
)
