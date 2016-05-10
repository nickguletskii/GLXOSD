cmake_minimum_required (VERSION 2.8)
project(GLXOSD-debian-packages LANGUAGES)

SET(DIST trusty CACHE STRING "Distributions to build for")
SET(DIST_VER 1 CACHE STRING "The version after distro name, e.g. +trusty1")
SET(ARCHS i386;amd64 CACHE STRING "Architectures to build for")
SET(SIGNING_KEY "" CACHE STRING "The key to sign changes with")
SET(GLXOSD_DEBIAN_VERSION 1 CACHE STRING "GLXOSD debian package version (e.g. 1)")
SET(GLXOSD_VERSION "3.1.2" CACHE STRING "GLXOSD version")
SET(GIT_REF "master" CACHE STRING "git ref to clone")
OPTION(BUILD_PBUILDERS OFF)
OPTION(CLONE OFF)


IF(${BUILD_PBUILDERS})
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
ENDIF()

SET(GLXOSD_DEBIAN_PACKAGE_ROOT ${CMAKE_BINARY_DIR}/packaging/)
SET(VERSION_STRING ${GLXOSD_VERSION}-${GLXOSD_DEBIAN_VERSION}+${DIST}${DIST_VER})
IF(${CLONE})
	execute_process(
		COMMAND git
			clone
			--recursive
			-b ${GIT_REF}
			git@github.com:nickguletskii/GLXOSD.git
			glxosd-${GLXOSD_VERSION}-orig
		WORKING_DIRECTORY
			${CMAKE_SOURCE_DIR})
	execute_process(
		COMMAND git
			pull
		WORKING_DIRECTORY
			${CMAKE_SOURCE_DIR}/glxosd-${GLXOSD_VERSION}-orig)
ENDIF()

configure_file (
	${CMAKE_SOURCE_DIR}/debian/control.in
	${CMAKE_SOURCE_DIR}/debian/control
	@ONLY
)
configure_file (
	${CMAKE_SOURCE_DIR}/debian/changelog.in
	${CMAKE_SOURCE_DIR}/debian/changelog
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
FOREACH(ARCH ${ARCHS})
	add_custom_command(
		TARGET debs PRE_BUILD
		COMMAND
				DIST=${DIST}
				ARCH=${ARCH}
				pdebuild
				--debbuildopts -sa
				--buildresult ${CMAKE_BINARY_DIR}/debs/
		WORKING_DIRECTORY
			${CMAKE_BINARY_DIR}/glxosd-${GLXOSD_VERSION}/
		)
ENDFOREACH()


add_custom_target(install_debs
	DEPENDS debs)
add_custom_command(
	TARGET install_debs POST_BUILD
	COMMAND
		sudo dpkg -i ${CMAKE_BINARY_DIR}/debs/glxosd-data_${VERSION_STRING}_all.deb)
FOREACH(ARCH ${ARCHS})
	add_custom_command(
	TARGET install_debs POST_BUILD
	COMMAND
		sudo dpkg -i ${CMAKE_BINARY_DIR}/debs/glxosd-libs-${ARCH}_${VERSION_STRING}_${ARCH}.deb)
ENDFOREACH()
add_custom_command(
	TARGET install_debs POST_BUILD
	COMMAND
	sudo dpkg -i ${CMAKE_BINARY_DIR}/debs/glxosd_${VERSION_STRING}_amd64.deb)

IF(NOT ${SIGNING_KEY} STREQUAL "")
	add_custom_target(sign_debs
		COMMAND
			debsign -k ${SIGNING_KEY} glxosd_${VERSION_STRING}_source.changes
		WORKING_DIRECTORY
			${CMAKE_BINARY_DIR})

	add_custom_target(push_debs
		COMMAND
			dput
			-f
			ppa:nickguletskii200/glxosd-experimental
			glxosd_${VERSION_STRING}_source.changes
		WORKING_DIRECTORY
			${CMAKE_BINARY_DIR})

	add_custom_target(reprepro_source
		COMMAND
			reprepro
			-b
			/var/repositories
			includedsc
			${DIST}
			glxosd_${VERSION_STRING}.dsc
		WORKING_DIRECTORY
			${CMAKE_BINARY_DIR})

	add_custom_target(reprepro_remove
		COMMAND
			reprepro
			-b
			/var/repositories
			remove
			${DIST}
			glxosd
			glxosd-data
			glxosd-libs-i386
			glxosd-libs-amd64
			glxosd-libs-nvidia-support-i386
			glxosd-libs-nvidia-support-amd64
			glxosd-libs-libsensors-support-i386
			glxosd-libs-libsensors-support-amd64
	)

	add_custom_target(reprepro_debs
		COMMAND
			reprepro
			-b
			/var/repositories
			includedeb
			${DIST}
			debs/glxosd_${VERSION_STRING}_amd64.deb
			debs/glxosd_${VERSION_STRING}_i386.deb
			debs/glxosd-data_${VERSION_STRING}_all.deb
			debs/glxosd-libs-amd64_${VERSION_STRING}_amd64.deb
			debs/glxosd-libs-i386_${VERSION_STRING}_i386.deb
			debs/glxosd-libs-libsensors-support-amd64_${VERSION_STRING}_amd64.deb
			debs/glxosd-libs-libsensors-support-i386_${VERSION_STRING}_i386.deb
			debs/glxosd-libs-nvidia-support-amd64_${VERSION_STRING}_amd64.deb
			debs/glxosd-libs-nvidia-support-i386_${VERSION_STRING}_i386.deb
		WORKING_DIRECTORY
			${CMAKE_BINARY_DIR})
ENDIF()
