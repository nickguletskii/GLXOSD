add_library(elfhacks SHARED elfhacks.cpp)
target_link_libraries(elfhacks dl rt)
set_target_properties(elfhacks PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR} LIBRARY_OUTPUT_NAME elfhacks)