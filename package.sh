#! /bin/bash

# Copyright (C) 2016 Nick Guletskii
#
# Permission is hereby granted,  free of charge,  to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to  use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.

DISTS=(trusty vivid wily xenial yakkety jessie stretch sid)
DEFAULTDIST=trusty
ARCHS=(i386 amd64)

TITLE="GLXOSD packaging helper"

function choose_distro {
	DIST_PARAMS=()
	for t in "${DISTS[@]}"; do
		DIST_PARAMS+=("$t" " " $([[ "${t}" = "${DEFAULTDIST}" ]] && echo "on" || echo "off"))
	done
	DISTS=$(whiptail --title "${TITLE}" --checklist "Choose distros to build for" 20 38 14 "${DIST_PARAMS[@]}" 3>&1 1>&2 2>&3)
	exitstatus=$?
	if [ $exitstatus != 0 ]; then
		exit
	fi
}

function build {
	GLXOSD_VERSION=$(whiptail --inputbox "Please enter the version of GLXOSD to create packages for:" 8 78 "${GLXOSD_VERSION}" --title "${TITLE}" 3>&1 1>&2 2>&3)
	exitstatus=$?
	if [ $exitstatus != 0 ]; then
		exit
	fi

	GLXOSD_DEBIAN_VERSION=$(whiptail --inputbox "Please enter the debian version of the GLXOSD packages:" 8 78 "${GLXOSD_DEBIAN_VERSION}" --title "${TITLE}" 3>&1 1>&2 2>&3)
	exitstatus=$?
	if [ $exitstatus != 0 ]; then
		exit
	fi

	CLONE=false
	if whiptail --title "${TITLE}" --defaultno --yesno "Clone/pull source?" 20 78; then
		CLONE=true
	fi

	GIT_REF="master"
	if $CLONE; then
		GIT_REF=$(whiptail --inputbox "Please enter the git ref to clone:" 8 78 "${GIT_REF}" --title "${TITLE}" 3>&1 1>&2 2>&3)
		exitstatus=$?
		if [ $exitstatus != 0 ]; then
			exit
		fi
	fi

	CREATE_PBUILDERS=false
	if whiptail --title "${TITLE}" --defaultno --yesno "Create pbuilders?" 20 78; then
		CREATE_PBUILDERS=true
	fi

	choose_distro

	SIGNING_KEY=$(whiptail --inputbox "Please enter the key to sign with:" 8 78 "${SIGNING_KEY}" --title "${TITLE}" 3>&1 1>&2 2>&3)
	exitstatus=$?
	if [ $exitstatus != 0 ]; then
		exit
	fi
	set -e
	for DIST in $DISTS
	do
		DIST=$(echo "${DIST}" | tr -d '"')

		mkdir -p build-${DIST}
		for ARCH in $ARCHS
		do
			cd build-${DIST}

			cmake -G "Unix Makefiles" \
				-DBUILD_PBUILDERS=$($CREATE_PBUILDERS && echo "ON" || echo "OFF") \
				-DCLONE=$($CLONE && echo "ON" || echo "OFF") \
				-DDIST="${DIST}" \
				-DGLXOSD_VERSION="${GLXOSD_VERSION}" \
				-DGLXOSD_DEBIAN_VERSION="${GLXOSD_DEBIAN_VERSION}" \
				-DSIGNING_KEY="${SIGNING_KEY}" \
				-DGIT_REF="${GIT_REF}" \
				..
			if [[ "${DIST}" == "${DEFAULTDIST}" ]]; then
				make install_debs
			else
				make debs
			fi
			cd ..
		done
	done
}

function sign {
	choose_distro

	for DIST in $DISTS
	do
		DIST=$(echo "${DIST}" | tr -d '"')

		for ARCH in $ARCHS
		do
			cd build-${DIST}
			make sign_debs
			cd ..
		done
	done
}

function reprepro_include {
	choose_distro

	for DIST in $DISTS
	do
		DIST=$(echo "${DIST}" | tr -d '"')

		for ARCH in $ARCHS
		do
			cd build-${DIST}
			make reprepro_source reprepro_debs
			cd ..
		done
	done
}

function reprepro_remove {
	choose_distro

	for DIST in $DISTS
	do
		DIST=$(echo "${DIST}" | tr -d '"')

		for ARCH in $ARCHS
		do
			cd build-${DIST}
			make reprepro_remove
			cd ..
		done
	done
}

function clean {
	choose_distro

	for DIST in $DISTS
	do
		DIST=$(echo "${DIST}" | tr -d '"')

		rm -rf build-${DIST}
	done
}

function upload {
	choose_distro

	for DIST in $DISTS
	do
		DIST=$(echo "${DIST}" | tr -d '"')

		for ARCH in $ARCHS
		do
			cd build-${DIST}
			make push_debs
			cd ..
		done
	done
}


ACTION=$(whiptail --title "${TITLE}" --menu "What would you like to do?" 20 78 8 \
"Build" "Build packages." \
"Sign" "Sign packages." \
"Reprepro include" "Add packages to repository using reprepro." \
"Reprepro remove" "Remove packages from repository using reprepro." \
"Upload" "Upload source packages to launchpad." \
"Clean" "Remove all build directories." 3>&1 1>&2 2>&3)
case $ACTION in
Build)
	build
	;;
Sign)
	sign
	;;
"Reprepro include")
	reprepro_include
	;;
"Reprepro remove")
	reprepro_remove
	;;
Upload)
	upload
	;;
Clean)
	clean
	;;
esac
