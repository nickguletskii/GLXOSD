#! /bin/bash
TEMPLATE=$(cat <<EOF
#! /bin/bash
GLXOSD_LIBRARY_PATH_I386=#{{GLXOSD_LIBRARY_PATH_I386}}
GLXOSD_LIBRARY_PATH_AMD64=#{{GLXOSD_LIBRARY_PATH_AMD64}}
if [ -d \$GLXOSD_LIBRARY_PATH_I386 ]; then
	GLXOSD_I386_PLUGIN_LIST=\$(ls \$GLXOSD_LIBRARY_PATH_I386/libglxosd-plugins-*.so | xargs -n 1 basename)
	GLXOSD_I386_PLUGIN_LIST=\$(echo \$GLXOSD_I386_PLUGIN_LIST | sed 's/ /:/g')
fi
if [ -d \$GLXOSD_LIBRARY_PATH_AMD64 ]; then
	GLXOSD_AMD64_PLUGIN_LIST=\$(ls \$GLXOSD_LIBRARY_PATH_AMD64/libglxosd-plugins-*.so | xargs -n 1 basename)
	GLXOSD_AMD64_PLUGIN_LIST=\$(echo \$GLXOSD_AMD64_PLUGIN_LIST | sed 's/ /:/g')
fi
export LD_LIBRARY_PATH=\$GLXOSD_LIBRARY_PATH_I386:\$GLXOSD_LIBRARY_PATH_AMD64:\$LD_LIBRARY_PATH
export LD_PRELOAD="libglxosd-elfhacks.so:libglxosd-glinject.so:libglxosd-ftgl.so:libglxosd.so:"\$LD_PRELOAD
GLXOSD_PLUGINS="\$GLXOSD_I386_PLUGIN_LIST:\$GLXOSD_AMD64_PLUGIN_LIST"
GLXOSD_PLUGINS=\$(echo "\$GLXOSD_PLUGINS" | awk -v RS=':' -v ORS=":" '!a[\$1]++')
export GLXOSD_PLUGINS
export GLINJECT_CONSTRUCTORS="constructGLXOSD"
export GLINJECT_DESTRUCTORS="destructGLXOSD"
\$@
EOF
)
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