add_library(glinject SHARED glinject.cpp)
target_link_libraries(glinject dl rt)
set_target_properties(glinject PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR} LIBRARY_OUTPUT_NAME glinject)