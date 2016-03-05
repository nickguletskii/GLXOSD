set(ELFHACKS_VERSION_MAJOR 1)
set(ELFHACKS_VERSION_MINOR 0)
set(ELFHACKS_VERSION_PATCH 1)
set(ELFHACKS_VERSION_STRING ${ELFHACKS_VERSION_MAJOR}.${ELFHACKS_VERSION_MINOR}.${ELFHACKS_VERSION_PATCH})
add_library(elfhacks SHARED elfhacks.cpp)
target_link_libraries(elfhacks dl rt)
set_target_properties(elfhacks PROPERTIES LIBRARY_OUTPUT_NAME glxosd-elfhacks)
set_target_properties(elfhacks PROPERTIES VERSION ${ELFHACKS_VERSION_STRING}
                                          SOVERSION ${ELFHACKS_VERSION_MAJOR})
INSTALL(TARGETS elfhacks
  LIBRARY DESTINATION lib/${CMAKE_LIBRARY_ARCHITECTURE}/glxosd
)
