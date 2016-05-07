include(GNUInstallDirs)
add_library(elfhacks STATIC elfhacks.c)
target_compile_options(elfhacks PRIVATE -fPIC)
target_link_libraries(elfhacks dl rt)
