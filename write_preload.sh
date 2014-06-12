#! /bin/bash
GLXOSD_SRC_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
TEMPLATE=$(cat ${GLXOSD_SRC_ROOT}/launcher_template.sh)
TARGET_FILE="./glxosd"
PREFIX="/usr"
while : ; do
	case "$1" in 
		--target-file)
		[ -n "${TARGET_FILE}" ]
			TARGET_FILE="$2"
			shift 2 ;;
		--installation-prefix)
		[ -n "${PREFIX}" ]
			PREFIX="$2"
			shift 2 ;;
		*)
			break ;;
	esac
done
function escape {
	echo $(echo $1 | sed -e 's/[\/&]/\\&/g')
}
OUTPUT=$TEMPLATE;
OUTPUT=$(echo "$OUTPUT" | sed "s/#{{GLXOSD_LIBRARY_PATH_I386}}/"$(escape $PREFIX)"\/lib\/i386-linux-gnu\/glxosd/g");
OUTPUT=$(echo "$OUTPUT" | sed "s/#{{GLXOSD_LIBRARY_PATH_AMD64}}/"$(escape $PREFIX)"\/lib\/x86_64-linux-gnu\/glxosd/g");
echo "$OUTPUT" > $TARGET_FILE;
chmod +x $TARGET_FILE