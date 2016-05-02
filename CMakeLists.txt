cmake_minimum_required (VERSION 2.8)
project(GLXOSD-debian-packages LANGUAGES)

SET(DISTS xenial CACHE STRING "Distributions to build for")
SET(ARCHS i386;amd64 CACHE STRING "Architectures to build for")
SET(SIGNING_KEY "" CACHE STRING "The key to sign changes with")
SET(GLXOSD_DEBIAN_VERSION 1 CACHE STRING "GLXOSD debian package version (e.g. 1)")
OPTION(BUILD_PBUILDERS OFF)
OPTION(CLONE OFF)

SET(GLXOSD_VERSION 3.1.0)

IF(${BUILD_PBUILDERS})
	FOREACH(DIST ${DISTS})
		FOREACH(ARCH ${ARCHS})
			SET(PBUILDER_PATH
				/var/cache/pbuilder/${DIST}-${ARCH}-base.tgz)
			execute_process(
				COMMAND
						sudo
						DIST=${DIST}
						ARCH=${ARCH}
						pbuilder
						--create
						--distribution ${DIST}
						--architecture ${ARCH}
						--basetgz ${PBUILDER_PATH}
				)
		ENDFOREACH()
	ENDFOREACH()
ENDIF()

SET(GLXOSD_DEBIAN_PACKAGE_ROOT ${CMAKE_BINARY_DIR}/packaging/)

IF(${CLONE})
	execute_process(
		COMMAND git
			clone
			--recursive
			-b v${GLXOSD_VERSION}
			git@github.com:nickguletskii/GLXOSD.git
			glxosd-${GLXOSD_VERSION}-orig
		WORKING_DIRECTORY
			${CMAKE_SOURCE_DIR})
ENDIF()

configure_file (
	${CMAKE_SOURCE_DIR}/debian/control.in
	${CMAKE_SOURCE_DIR}/debian/control
	@ONLY
)

add_custom_target(original_tar_gz
	COMMAND
		tar
		--exclude=".git" --exclude="src/freetype-gl-glxosd/freetype-gl/fonts"
		-vcJf
		${CMAKE_BINARY_DIR}/glxosd_${GLXOSD_VERSION}.orig.tar.xz
		.
	WORKING_DIRECTORY
		${CMAKE_SOURCE_DIR}/glxosd-${GLXOSD_VERSION}-orig)

add_custom_target(pbuilder_workdir
	COMMAND
		rsync
		-abrv
		--progress
		--force
		--delete
		--delete-excluded
		--exclude .git/
		${CMAKE_SOURCE_DIR}/glxosd-${GLXOSD_VERSION}-orig/
		${CMAKE_BINARY_DIR}/glxosd-${GLXOSD_VERSION}
	COMMAND
		rsync
		-abrv
		--progress
		--force
		--delete
		--delete-excluded
		${CMAKE_SOURCE_DIR}/debian
		${CMAKE_BINARY_DIR}/glxosd-${GLXOSD_VERSION}
)


add_custom_target(debs
	DEPENDS pbuilder_workdir original_tar_gz)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/debs/)
FOREACH(DIST ${DISTS})
	FOREACH(ARCH ${ARCHS})
		add_custom_command(
			TARGET debs PRE_BUILD
			COMMAND
					DIST=${DIST}
					ARCH=${ARCH}
					pdebuild
					--buildresult ${CMAKE_BINARY_DIR}/debs/
			WORKING_DIRECTORY
				${CMAKE_BINARY_DIR}/glxosd-${GLXOSD_VERSION}/
			)
	ENDFOREACH()
ENDFOREACH()


add_custom_target(install_debs
	DEPENDS debs)
add_custom_command(
	TARGET install_debs POST_BUILD
	COMMAND
		sudo dpkg -i ${CMAKE_BINARY_DIR}/debs/glxosd-data_${GLXOSD_VERSION}-${GLXOSD_DEBIAN_VERSION}_all.deb)
FOREACH(ARCH ${ARCHS})
	add_custom_command(
	TARGET install_debs POST_BUILD
	COMMAND
		sudo dpkg -i ${CMAKE_BINARY_DIR}/debs/glxosd-libs-${ARCH}_${GLXOSD_VERSION}-${GLXOSD_DEBIAN_VERSION}_${ARCH}.deb)
ENDFOREACH()
add_custom_command(
	TARGET install_debs POST_BUILD
	COMMAND
	sudo dpkg -i ${CMAKE_BINARY_DIR}/debs/glxosd_${GLXOSD_VERSION}-${GLXOSD_DEBIAN_VERSION}_amd64.deb)

IF(NOT ${SIGNING_KEY} STREQUAL "")
	add_custom_target(sign_debs
		DEPENDS install_debs
		COMMAND
			debsign -k ${SIGNING_KEY} glxosd_${GLXOSD_VERSION}-${GLXOSD_DEBIAN_VERSION}.dsc
		WORKING_DIRECTORY
			${CMAKE_BINARY_DIR}/debs/)

	add_custom_target(push_debs
		COMMAND
			dput
			ppa:nickguletskii200/glxosd-experimental
			glxosd_${GLXOSD_VERSION}-${GLXOSD_DEBIAN_VERSION}.dsc
		WORKING_DIRECTORY
			${CMAKE_BINARY_DIR}/debs/)
ENDIF()
