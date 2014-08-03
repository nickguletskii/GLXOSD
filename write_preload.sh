#! /bin/bash
GLXOSD_SRC_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
TEMPLATE=$(cat ${GLXOSD_SRC_ROOT}/launcher_template.sh)
TARGET_FILE="./glxosd"
while : ; do
	case "$1" in 
		--target-file)
		[ -n "${TARGET_FILE}" ]
			TARGET_FILE="$2"
			shift 2 ;;
		--installation-prefix)
		[ -n "${INSTALL_PREFIX}" ]
			INSTALL_PREFIX="$2"
			shift 2 ;;
		--installation-suffix-32)
		[ -n "${INSTALL_SUFFIX_32}" ]
			INSTALL_SUFFIX_32="$2"
			shift 2 ;;
		--installation-suffix-64)
		[ -n "${INSTALL_SUFFIX_64}" ]
			INSTALL_SUFFIX_64="$2"
			shift 2 ;;
		*)
			break ;;
	esac
done
function escape {
	echo $(echo $1 | sed -e 's/[\/&]/\\&/g')
}
OUTPUT="${TEMPLATE}";
OUTPUT=$(echo "${OUTPUT}" | sed "s/#{{GLXOSD_LIBRARY_PATH_I386}}/"$(escape ${INSTALL_PREFIX-"/usr/"})"\/"$(escape ${INSTALL_SUFFIX_32-"/lib/i386-linux-gnu/"})"\/gloxsd\//g");
OUTPUT=$(echo "${OUTPUT}" | sed "s/#{{GLXOSD_LIBRARY_PATH_AMD64}}/"$(escape ${INSTALL_PREFIX-"/usr/"})"\/"$(escape ${INSTALL_SUFFIX_64-"/lib/x86_64-linux-gnu/"})"\/gloxsd\//g");
echo "${OUTPUT}" > "${TARGET_FILE}"
chmod +x "${TARGET_FILE}"