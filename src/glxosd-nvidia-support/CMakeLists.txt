include(GNUInstallDirs)
SET(GLXOSD_NVIDIA_SUPPORT_SOURCE_FILES NvidiaSensorDataProvider.cpp)
add_library(glxosd-nvidia-support SHARED ${GLXOSD_NVIDIA_SUPPORT_SOURCE_FILES})
add_dependencies(glxosd-nvidia-support glxosd)
target_link_libraries(glxosd-nvidia-support dl rt ${Boost_LIBRARIES} ${X11_LIBRARIES} ${NVCTRL_LIBRARIES} ${OUTPUT_DIR}/libglxosd.so)
set_target_properties(glxosd-nvidia-support PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR} LIBRARY_OUTPUT_NAME glxosd-plugins-nvidia-support)
set_target_properties(glxosd-nvidia-support PROPERTIES VERSION ${GLXOSD_VERSION_STRING}
                                          SOVERSION ${GLXOSD_VERSION_MAJOR})
INSTALL(TARGETS glxosd-nvidia-support
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/glxosd
)
