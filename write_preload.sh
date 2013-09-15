#! /bin/sh
TARGET_DIR=$1
PREFIX=$2
ARCH_SUFFIX=$3
PLATFORM_SUFFIX=$4
OUT_FILE=$TARGET_DIR/glxosd$PLATFORM_SUFFIX
echo "#! /bin/sh" > ${OUT_FILE}
echo "LD_LIBRARY_PATH="$PREFIX"/lib/"$ARCH_SUFFIX"/glxosd:\$LD_LIBRARY_PATH LD_PRELOAD=libglxosd-elfhacks.so:libglxosd-glinject.so:libglxosd.so:libglxosd-ftgl.so:\$LD_PRELOAD \$@" >> ${OUT_FILE}
chmod +x ${OUT_FILE}
OUT_FILE=$TARGET_DIR/glxosd
echo "#! /bin/sh" > ${OUT_FILE}
if [ $PLATFORM_SUFFIX = "-amd64" ]; then
    echo "LD_LIBRARY_PATH="$PREFIX"/lib/x86_64-linux-gnu/glxosd:"$PREFIX"/lib/i386-linux-gnu/glxosd:\$LD_LIBRARY_PATH LD_PRELOAD=libglxosd-elfhacks.so:libglxosd-glinject.so:libglxosd.so:libglxosd-ftgl.so:\$LD_PRELOAD \$@" >> ${OUT_FILE}
elif [ $PLATFORM_SUFFIX = "-i386" ]; then
    echo "LD_LIBRARY_PATH="$PREFIX"/lib/i386-linux-gnu/glxosd:\$LD_LIBRARY_PATH LD_PRELOAD=libglxosd-elfhacks.so:libglxosd-glinject.so:libglxosd.so:libglxosd-ftgl.so:\$LD_PRELOAD \$@" >> ${OUT_FILE}
fi
chmod +x ${OUT_FILE}
