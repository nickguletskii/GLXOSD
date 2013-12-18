##### Add include paths #####
include_directories(${PROJECT_SOURCE_DIR}/src/elfhacks)
include_directories(${PROJECT_SOURCE_DIR}/src/ftgl)
include_directories(${PROJECT_SOURCE_DIR}/src/glinject)
include_directories(${PROJECT_SOURCE_DIR}/src/glxosd)


##### Descend into components #####
add_subdirectory(elfhacks)
add_subdirectory(ftgl)
add_subdirectory(glinject)
add_subdirectory(glxosd)

##### Plugins #####
if(NVCTRL_FOUND)
	include_directories(${PROJECT_SOURCE_DIR}/src/glxosd-nvidia-support)
	add_subdirectory(glxosd-nvidia-support)
endif(NVCTRL_FOUND)

if(SENSORS_FOUND)
	include_directories(${PROJECT_SOURCE_DIR}/src/glxosd-libsensors-support)
	add_subdirectory(glxosd-libsensors-support)
endif(SENSORS_FOUND)