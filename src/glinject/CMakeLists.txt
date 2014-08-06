set(GLINJECT_VERSION_MAJOR 2)
set(GLINJECT_VERSION_MINOR 1)
set(GLINJECT_VERSION_PATCH 1)
set(GLINJECT_VERSION_STRING ${GLINJECT_VERSION_MAJOR}.${GLINJECT_VERSION_MINOR}.${GLINJECT_VERSION_PATCH})
add_library(glinject SHARED glinject.cpp)
add_dependencies(glinject elfhacks)
target_link_libraries(glinject dl rt ${OUTPUT_DIR}/libglxosd-elfhacks.so)
set_target_properties(glinject PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR} LIBRARY_OUTPUT_NAME glxosd-glinject)
set_target_properties(glinject PROPERTIES VERSION ${GLINJECT_VERSION_STRING}
                                          SOVERSION ${GLINJECT_VERSION_MAJOR})
INSTALL(TARGETS glinject
  LIBRARY DESTINATION lib/${CMAKE_LIBRARY_ARCHITECTURE}/glxosd
)