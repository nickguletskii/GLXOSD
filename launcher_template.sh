#! /bin/bash

function showHelp() {
	echo "Usage: glxosd [options] command"
	echo "	-h or --help : show this help message"
	echo "	-c <path> or --config <path> : specify the path to the config"
	echo "	-s or --steam : inject Steam overlay"
	echo "	--steam-path <path> : set path to Steam"
	echo "Also, you can set GLXOSD_PRELOAD in the same way as LD_PRELOAD, except that GLXOSD_PRELOAD will be in LD_PRELOAD before GLXOSD libraries."
}

OPTIND=1

INJECT_STEAM_OVERLAY=false
GLXOSD_PRELOAD=""
STEAM_PATH=~/.local/share/Steam

while : ; do
	case "$1" in 
		-h|--help)
			showHelp;
			exit 0;;
		-c|--config)
			export GLXOSD_CONFIG_PATH="$2"
			shift 2 ;;
		-s|--steam)
			INJECT_STEAM_OVERLAY=true;
			shift 1;;
		--steam-path)
			STEAM_PATH="$2"
			shift 2 ;;
		*)
			break;;
	esac
done
STEAM_OVERLAY_LIB_PATHS=""
STEAM_OVERLAY_LIBS=""
if [ "$INJECT_STEAM_OVERLAY" = true ]; then
	STEAM_OVERLAY_LIBS="gameoverlayrenderer.so"
	for dir in $(find ${STEAM_PATH} -maxdepth 1 -name "*_32"); do
		STEAM_OVERLAY_LIB_PATHS="${STEAM_OVERLAY_LIB_PATHS}:${dir}"
	done
	for dir in $(find ${STEAM_PATH} -maxdepth 1  -name "*_64"); do
		STEAM_OVERLAY_LIB_PATHS="${STEAM_OVERLAY_LIB_PATHS}:${dir}"
	done
fi

GLXOSD_LIBRARY_PATH_I386="{{GLXOSD_LIBRARY_PATH_I386}}"
GLXOSD_LIBRARY_PATH_AMD64="{{GLXOSD_LIBRARY_PATH_AMD64}}"

export LD_LIBRARY_PATH="${STEAM_OVERLAY_LIB_PATHS}:${GLXOSD_LIBRARY_PATH_I386}:${GLXOSD_LIBRARY_PATH_AMD64}:${LD_LIBRARY_PATH}"
export LD_PRELOAD="${GLXOSD_PRELOAD}:${STEAM_OVERLAY_LIBS}:libglxosd-elfhacks.so:libglxosd-glinject.so:libglxosd-freetype-gl.so:libluajit-5.1.so:${LD_PRELOAD}"
export GLINJECT_BOOTSTRAP_SCRIPT="{{GLXOSD_SHARED_PATH}}/glxosd/init.lua"

export GLINJECT_CONSTRUCTORS="constructGLXOSD"
export GLINJECT_DESTRUCTORS="destructGLXOSD"

"$@" 
